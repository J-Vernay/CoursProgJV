/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>

#include <format>
#include <algorithm>
#include <optional>


constexpr double SPAWNTIME_BEGIN = 1;
constexpr double SPAWNTIME_END = 0.033;
constexpr double CHRONO_INIT = 60;

// Variables globales.
double g_lastTime = 0;

std::vector<DinoPlayer> g_Players;
DinoTerrain g_Terrain;
std::vector<DinoLasso> g_Lassos;
std::vector<DinoAnimal> g_Animals;
double g_timeSpawnAnimal = 0;
double g_chrono = CHRONO_INIT;
bool g_bWasStartPressed = false;
bool g_bPause = false;

std::optional<DinoVertexBuffer> g_vbufID_prenom;
DinoVec2 textSize_prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

constexpr DinoVec2 RENDER_SIZE = {480, 360};


void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);

    DinoPlayer::InitStatic();
    DinoAnimal::InitStatic();

    // Resize() appelle le constructeur par défaut;
    // il n'y a pas de constructeur par défaut dans DinoPlayer
    //g_Players.resize(4);
    g_Players.emplace_back(0);
    g_Players.emplace_back(1);
    g_Players.emplace_back(2);
    g_Players.emplace_back(3);

    g_Lassos.emplace_back(DinoColor_BLUE);
    g_Lassos.emplace_back(DinoColor_RED);
    g_Lassos.emplace_back(DinoColor_YELLOW);
    g_Lassos.emplace_back(DinoColor_GREEN);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_Terrain.Init(RENDER_SIZE, idxSeason);

    // Préparation du drawcall du prénom
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "Julien VERNAY", DinoColor_WHITE, DinoColor_GREY);
        g_vbufID_prenom.emplace(vs.data(), vs.size(), "Prenom");
    }

}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    XDino_SetRenderSize(RENDER_SIZE);

    // Gestion des entrées et mise à jour de la logique de jeu.

    DinoGamepad gamepads[4];
    DinoGamepad gamepad;
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad))
        gamepads[0] = gamepad;
    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad))
        gamepads[1] = gamepad;
    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad))
        gamepads[2] = gamepad;
    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad))
        gamepads[3] = gamepad;

    bool bPressedStart = false;
    for (DinoGamepad& g : gamepads)
        bPressedStart = bPressedStart || g.start;

    if (bPressedStart && !g_bWasStartPressed)
        g_bPause = !g_bPause; // g_bPause prend l'inverse de g_bPause
    g_bWasStartPressed = bPressedStart;

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();

    if (!g_bPause) {
        for (int i = 0; i < 4; ++i)
            g_Players[i].Update(timeSinceStart, deltaTime, gamepads[i]);

        // Purger les animaux qui sont morts.
        // /!\ std::remove ne supprime pas /!\ il déplace à la fin du tableau
        // Il faut ensuite appeler .erase() pour enlever les éléments.
        auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
        for (auto it2 = it; it2 < g_Animals.end(); ++it2)
            it2->Shut();
        g_Animals.erase(it, g_Animals.end());

        // Spawner un animal si besoin.
        if (timeSinceStart > g_timeSpawnAnimal) {
            EAnimalKind kind = (EAnimalKind)XDino_RandomInt32(0, 7);

            float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
            float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);

            DinoAnimal& animal = g_Animals.emplace_back(timeSinceStart, kind, DinoVec2{x, y});
            double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * g_chrono;
            g_timeSpawnAnimal = timeSinceStart + spawnTime;
        }

        // Update les animaux.
        for (DinoAnimal& animal : g_Animals)
            animal.Update(timeSinceStart, deltaTime);
    }

    // Pointeur de DinoEntity peut pointer vers DinoPlayer/DinoAnimal
    // car il y a un lien d'héritage.
    std::vector<DinoEntity*> entities;
    for (DinoPlayer& player : g_Players)
        entities.emplace_back(&player);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);

    if (!g_bPause) {
        for (size_t idxA = 0; idxA < entities.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
                DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

        for (DinoEntity* pEntity : entities)
            pEntity->ApplyLimit(terrainMin, terrainMax);

        if (g_Lassos.size() != g_Players.size())
            DINO_CRITICAL("Il devrait y avoir autant de lassos que de joueurs");
        for (int i = 0; i < g_Lassos.size(); ++i)
            g_Lassos[i].Update(g_Players[i].GetPos());

        for (size_t idxA = 0; idxA < g_Lassos.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < g_Lassos.size(); ++idxB)
                DinoLasso::ResolveCollision(g_Lassos[idxA], g_Lassos[idxB]);

        for (DinoLasso& lasso : g_Lassos)
            for (DinoEntity* pEntity : entities)
                if (lasso.WasInLoop(pEntity->GetPos()))
                    pEntity->ReactLoop(timeSinceStart);

        // Décrémenter le chronomètre.
        g_chrono -= deltaTime;
    }

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    g_Terrain.Draw();

    for (DinoLasso& lasso : g_Lassos)
        lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);

        // vs : Stocke une allocation mémoire sur le CPU
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);

        // vbufID : Représente une allocation mémoire sur la carte graphique
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "dTime");

        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {}, 2);

        // Destructeur de 'vbuf' appelé implicitement par le compilateur
        // Destructeur de 'vs' appelé implicitement par le compilateur
    }

    if (g_bPause) {
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, "-- PAUSE --", DinoColor_WHITE, DinoColor_BLACK);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (RENDER_SIZE.x - textSize.x * 6) / 2;
        float ty = (RENDER_SIZE.y - textSize.y * 6) / 2;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 6);
    }
    {
        std::string text = std::format("{:.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (RENDER_SIZE.x - textSize.x * 2) / 2;
        float ty = 0;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

    // Affiche le prénom.
    {
        float tx = (RENDER_SIZE.x - textSize_prenom.x * 2);
        float ty = (RENDER_SIZE.y - textSize_prenom.y * 2);
        XDino_Draw(g_vbufID_prenom->Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur CTRL.
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
    // For-range loop
    for (DinoPlayer& player : g_Players)
        player.Shut();
    for (DinoAnimal& animal : g_Animals)
        animal.Shut();
    g_Terrain.Shut();

    DinoPlayer::ShutStatic();
    DinoAnimal::ShutStatic();

    g_vbufID_prenom.reset();
}