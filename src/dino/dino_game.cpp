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

std::optional<DinoVertexBuffer> g_vbufID_prenom;
DinoVec2 textSize_prenom;

// Pause
bool g_paused = false;
bool g_prevStartPressed[4] = {false, false, false, false};
double g_lastTimeActive = 0; // dernier timeSinceStart actif avant pause

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
        textSize_prenom = Dino_GenVertices_Text(vs, "Remi CHAUVIN", DinoColor_WHITE, DinoColor_GREY);
        g_vbufID_prenom.emplace(vs.data(), vs.size(), "Prenom");
    }
    g_lastTimeActive = 0;
}

void Dino_GameFrame(double timeSinceStart)
{
    // Calcul du deltaTime
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    XDino_SetRenderSize(RENDER_SIZE);

    // Récupération des gamepads
    DinoGamepad gamepad[4];
    XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad[0]);
    XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad[1]);
    XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad[2]);
    XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad[3]);

    // Gestion du bouton START pour pause / unpause
    for (int i = 0; i < 4; ++i) {
        if (gamepad[i].start && !g_prevStartPressed[i]) {
            g_paused = !g_paused;
        }
        g_prevStartPressed[i] = gamepad[i].start;
    }

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();

    // Update logique si le jeu n'est pas en pause
    if (!g_paused) {

        // Supprime les animaux morts
        auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
        for (auto it2 = it; it2 < g_Animals.end(); ++it2)
            it2->Shut();
        g_Animals.erase(it, g_Animals.end());

        // Spawn d'animaux si nécessaire
        if (timeSinceStart > g_timeSpawnAnimal) {
            EAnimalKind kind = (EAnimalKind)XDino_RandomInt32(0, 7);
            float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
            float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);
            DinoAnimal& animal = g_Animals.emplace_back(timeSinceStart, kind, DinoVec2{x, y});
            double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * g_chrono;
            g_timeSpawnAnimal = timeSinceStart + spawnTime;
        }

        // Update des lassos
        for (int i = 0; i < g_Lassos.size(); ++i)
            g_Lassos[i].Update(g_Players[i].GetPos());

        // Update des joueurs
        for (int i = 0; i < g_Players.size(); ++i)
            g_Players[i].Update(timeSinceStart, deltaTime, gamepad[i]);

        // Update des animaux
        for (DinoAnimal& animal : g_Animals)
            animal.Update(timeSinceStart, deltaTime);

        // Décrémente le chrono
        g_chrono -= deltaTime;
    }

    // Collision et limites
    std::vector<DinoEntity*> entities;
    for (DinoPlayer& player : g_Players)
        entities.emplace_back(&player);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);

    for (size_t idxA = 0; idxA < entities.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
            DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

    for (DinoEntity* pEntity : entities)
        pEntity->ApplyLimit(terrainMin, terrainMax);

    for (size_t idxA = 0; idxA < g_Lassos.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < g_Lassos.size(); ++idxB)
            DinoLasso::ResolveCollision(g_Lassos[idxA], g_Lassos[idxB]);

    // ReactLoop seulement si pas en pause
    if (!g_paused) {
        for (DinoLasso& lasso : g_Lassos)
            for (DinoEntity* pEntity : entities)
                if (lasso.WasInLoop(pEntity->GetPos()))
                    pEntity->ReactLoop(timeSinceStart);
    }

    // Tri vertical pour affichage correct
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

    // Affichage du chrono
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

    // Affiche "PAUSE" si en pause
    if (g_paused) {
        std::vector<DinoVertex> vs;
        DinoVec2 pauseSize = Dino_GenVertices_Text(vs, "PAUSE", DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Pause");
        float tx = (RENDER_SIZE.x - pauseSize.x * 2) / 2;
        float ty = (RENDER_SIZE.y - pauseSize.y * 2) / 2;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 3);
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