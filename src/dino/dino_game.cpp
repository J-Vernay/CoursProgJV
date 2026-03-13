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
double g_LastPauseTime = 0;

std::vector<DinoGamepadIdx> g_gamepads;
std::vector<DinoPlayer> g_Players;
DinoTerrain g_Terrain;
std::vector<DinoLasso> g_Lassos;
std::vector<DinoAnimal> g_Animals;
double g_timeSpawnAnimal = 0;
double g_chrono = CHRONO_INIT;

std::optional<DinoVertexBuffer> g_vbufID_prenom;
DinoVec2 textSize_prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

bool g_pauseGame = false;
bool g_inLobby = true;

constexpr DinoVec2 RENDER_SIZE = {480, 360};


void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);

    DinoPlayer::InitStatic();
    DinoAnimal::InitStatic();

    g_gamepads.emplace_back(DinoGamepadIdx::Gamepad1);
    g_gamepads.emplace_back(DinoGamepadIdx::Gamepad2);
    g_gamepads.emplace_back(DinoGamepadIdx::Gamepad3);
    g_gamepads.emplace_back(DinoGamepadIdx::Gamepad4);
    g_gamepads.emplace_back(DinoGamepadIdx::Keyboard);
    
    // Resize() appelle le constructeur par défaut;
    // il n'y a pas de constructeur par défaut dans DinoPlayer
    //g_Players.resize(4);
    g_Players.emplace_back(0);
    //g_Players.emplace_back(1);
    //g_Players.emplace_back(2);
    //g_Players.emplace_back(3);

    g_Lassos.emplace_back(DinoColor_BLUE);
   // g_Lassos.emplace_back(DinoColor_RED);
   // g_Lassos.emplace_back(DinoColor_YELLOW);
   // g_Lassos.emplace_back(DinoColor_GREEN);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_Terrain.Init(RENDER_SIZE, idxSeason);

    // Préparation du drawcall du prénom
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "BOULANGER Antoine", DinoColor_WHITE, DinoColor_GREY);
        g_vbufID_prenom.emplace(vs.data(), vs.size(), "Prenom");
    }

}

void Dino_GameFrame(double timeSinceStart)
{
    
    float deltaTime = g_pauseGame ? 0 : static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime =  timeSinceStart;
    if(!g_pauseGame)
        g_LastPauseTime = timeSinceStart;
    

    XDino_SetRenderSize(RENDER_SIZE);
    DinoGamepad gamepad{};
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad))
        g_Players[0].Update(timeSinceStart, deltaTime, gamepad, g_pauseGame);

    // if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad))
    //     g_Players[1].Update(timeSinceStart, deltaTime, gamepad, g_pauseGame);
    //
    // if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad))
    //     g_Players[2].Update(timeSinceStart, deltaTime, gamepad, g_pauseGame);
    //
    // if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad))
    //     g_Players[3].Update(timeSinceStart, deltaTime, gamepad, g_pauseGame);

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();
    
    auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
    for (auto it2 = it; it2 < g_Animals.end(); ++it2)
        it2->Shut();
    g_Animals.erase(it, g_Animals.end());

    
    if ((timeSinceStart > g_timeSpawnAnimal) && !g_pauseGame) {
        auto kind = static_cast<EAnimalKind>(XDino_RandomInt32(0, 7));

        float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
        float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);

        DinoAnimal& animal = g_Animals.emplace_back(timeSinceStart, kind, DinoVec2{x, y});
        double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * g_chrono;
        g_timeSpawnAnimal = timeSinceStart + spawnTime;
    }

    
    for (DinoAnimal& animal : g_Animals)
        animal.Update(timeSinceStart, deltaTime);
    
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

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    
    g_chrono -= deltaTime;
    
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);
    
    g_Terrain.Draw(g_pauseGame ? g_LastPauseTime : timeSinceStart);

    for (DinoLasso& lasso : g_Lassos)
        lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(g_pauseGame ?  g_LastPauseTime :timeSinceStart);

  
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {}, 2);
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