/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_animal.h"
#include "dino_lasso.h"

#include <algorithm>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <map>
#include <dino/DinoController.h>
#include <dino/dino_terrain.h>

#include <format>

constexpr double SPAWNTIME_BEGIN = 1;
constexpr double SPAWNTIME_END = 0.2;
constexpr double CHRONO_INIT = 60;

// Variables globales.
double g_lastTime = 0;

uint64_t vbufID_prenom;
DinoVec2 textSize_Prenom;

std::map<DinoGamepadIdx, DinoControllerFields> GamepadControllers;
std::vector<DinoControllerFields*> g_players;
uint64_t texID_dino;

std::vector<DinoLasso> g_Lassos;

DinoTerrain g_terrain;

std::vector<DinoAnimal> g_Animals;
double g_timeSpawnAnimal = 0;
double g_chrono = CHRONO_INIT;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

void Dino_GameInit()
{
    // DinoVec2 windowSize = XDino_GetWindowSize();
    DinoVec2 windowSize = {480, 360};
    XDino_SetRenderSize(windowSize);

    int playerCount = 0;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& controller = GamepadControllers[gamepadIdx];
        controller = {};

        g_players.emplace_back(&controller);

        controller.Init(playerCount);
        playerCount++;
    }

    g_Lassos.resize(g_players.size());
    if (g_players.size() >= 1)
        g_Lassos[0].Init(DinoColor_BLUE);
    if (g_players.size() >= 2)
        g_Lassos[1].Init(DinoColor_RED);
    if (g_players.size() >= 3)
        g_Lassos[2].Init(DinoColor_YELLOW);
    if (g_players.size() >= 4)
        g_Lassos[3].Init(DinoColor_GREEN);

    // Préparation du drawcall du nom en bas à droite
    {
        std::vector<DinoVertex> vs;
        textSize_Prenom = Dino_GenVertices_Text(vs, "Cedric Charrier", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

    // Preparing the texture for the dino
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");

    g_terrain.Init(DinoVec2{24, 16}, 10);

    // Preparing texture of animals
    DinoAnimal::InitTexture();
    DinoControllerFields::InitTexture();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.start)
            return;
    }

    // Gestion des entrées et mise à jour de la logique des dinos.
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& controller = GamepadControllers[gamepadIdx];

        controller.DinoMovement(gamepad, deltaTime);
    }

    // for (DinoControllerFields* playerA : g_players) {
    //     for (DinoControllerFields* playerB : g_players) {
    //         if (playerA == playerB)
    //             continue;
    //
    //         playerA->CheckForOtherLassIntersections(*playerB);
    //     }
    // }

    // Managing animal logic

    // Purger les animaux qui sont morts.
    // /!\ std::remove ne supprime pas /!\ il déplace à la fin du tableau
    // Il faut ensuite appeler .erase() pour enlever les éléments.
    auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
    for (auto it2 = it; it2 < g_Animals.end(); ++it2)
        it2->Shut();
    g_Animals.erase(it, g_Animals.end());

    if (timeSinceStart > g_timeSpawnAnimal) {
        DinoAnimal& animal = g_Animals.emplace_back();
        EAnimalKind kind = (EAnimalKind)XDino_RandomInt32(0, 7);

        DinoVec2 min = g_terrain.GetTopLeft();
        DinoVec2 max = g_terrain.GetBottomRight();
        float x = XDino_RandomFloat(min.x, max.x);
        float y = XDino_RandomFloat(min.y, max.y);

        animal.Init(kind, {x, y}, timeSinceStart);
        double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * g_chrono;
        g_timeSpawnAnimal = timeSinceStart + spawnTime;
    }
    for (DinoAnimal& animal : g_Animals)
        animal.Update(deltaTime);

    // Pointeur de DinoEntity peut pointer vers DinoPlayer/DinoAnimal
    // car il y a un lien d'héritage.
    std::vector<DinoEntity*> entities;
    for (DinoControllerFields* player : g_players)
        entities.emplace_back(player);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);

    for (size_t idxA = 0; idxA < entities.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
            DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

    for (DinoEntity* pEntity : entities)
        pEntity->ApplyTerrainLimit(g_terrain);

    if (g_Lassos.size() != g_players.size())
        DINO_CRITICAL("Il devrait y avoir autant de lassos que de joueurs");
    for (int i = 0; i < g_Lassos.size(); ++i)
        g_Lassos[i].Update(g_players[i]->GetPos());

    for (size_t idxA = 0; idxA < g_Lassos.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < g_Lassos.size(); ++idxB)
            DinoLasso::ResolveCollision(g_Lassos[idxA], g_Lassos[idxB]);

    for (DinoLasso& lasso : g_Lassos)
        for (DinoEntity* pEntity : entities)
            if (lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart);

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    // Décrémenter le chronomètre.
    g_chrono -= deltaTime;

    // -- Affichage -- 

    g_terrain.DrawBG();
    g_terrain.DrawTerrain();
    g_terrain.DrawFlwrs();

    for (DinoLasso& lasso : g_Lassos)
        lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);

    // Dessin de la "polyligne"
    // XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // // Drawing Animals
    // for (DinoAnimal& animal : g_Animals)
    //     animal.Draw(timeSinceStart);
    //
    // // Drawing Dinos
    // for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
    //
    //     DinoGamepad gamepad{};
    //     bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
    //     if (!bSuccess)
    //         continue;
    //
    //     DinoControllerFields& controller = GamepadControllers[gamepadIdx];
    //
    //     controller.Draw(deltaTime);
    // }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    DinoVec2 renderSize = XDino_GetRenderSize();
    // Chrono
    {
        std::string text = std::format("{:.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (renderSize.x - textSize.x * 2) / 2;
        float ty = 0;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

    // Affichage du nom en bas à droite
    {
        float tx = (renderSize.x - textSize_Prenom.x * 2);
        float ty = (renderSize.y - textSize_Prenom.y * 2);

        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {.x = tx, .y = ty}, 2);
    }

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur SHIFT.
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.shoulder_left) {
        int diff = 0;
        if (keyboard.dpad_up)
            diff -= 1;
        if (keyboard.dpad_down)
            diff += 1;
        XDino_DrawStats(diff);
    }
#endif
}

void Dino_GameShut()
{
    XDino_DestroyVertexBuffer(vbufID_prenom);

    XDino_DestroyGpuTexture(texID_dino);
    DinoAnimal::ShutTexture();
    DinoControllerFields::ShutTexture();

    g_terrain.Shut();
}