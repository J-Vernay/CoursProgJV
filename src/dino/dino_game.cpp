/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <algorithm>
#include <dino/dino_terrain.h>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <iostream>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;
uint64_t vbufID_polyline;
uint64_t vbuffID_nom;
DinoVec2 text_Size_nom;
anim Get_Current_Anim();
std::vector<DinoPlayer> g_players = {};
std::vector<DinoLasso> g_Lassos;
DinoTerrain g_terrain;
DinoAnimalSpawner g_spawner;
double g_chrono = 60;
// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr DinoVec2 RENDER_SIZE = {480, 360};


void Dino_GameInit()
{

    g_players.resize(4);
    g_players[0].Init(0);
    g_players[1].Init(1);
    g_players[2].Init(2);
    g_players[3].Init(3);

    g_Lassos.resize(4);
    g_Lassos[0].Init(DinoColor_BLUE);
    g_Lassos[1].Init(DinoColor_RED);
    g_Lassos[2].Init(DinoColor_YELLOW);
    g_Lassos[3].Init(DinoColor_GREEN);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_terrain.Init(RENDER_SIZE, idxSeason);
    g_spawner.Init();
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    // Préparation du drawcall de la polyline (zigzag en fond).
    {
        constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

        std::vector<DinoVec2> polyline;
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Polyline(vs, polyline, 100, POLYLINE_COLOR);
        vbufID_polyline = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Polyline");
    }

    // Set up Anims.

    // Préparation du drawCall du nom
    {
        std::vector<DinoVertex> vs;
        text_Size_nom = Dino_GenVertices_Text(vs, "Betremieux Elouan", DinoColor_WHITE, DinoColor_GREY);
        vbuffID_nom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

}


void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    XDino_SetRenderSize({480, 360});
    // Gestion des entrées et mise à jour de la logique de jeu.

    DinoGamepad gamepad{};
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad))
        g_players[0].Update(timeSinceStart, deltaTime, g_terrain, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad))
        g_players[1].Update(timeSinceStart, deltaTime, g_terrain, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad))
        g_players[2].Update(timeSinceStart, deltaTime, g_terrain, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad))
        g_players[3].Update(timeSinceStart, deltaTime, g_terrain, gamepad);

    auto it = std::remove_if(g_spawner.m_animals.begin(), g_spawner.m_animals.end(), DinoAnimal::IsDead);
    for (auto it2 = it; it2 < g_spawner.m_animals.end(); ++it2)
        it2->Shut();

    g_spawner.m_animals.erase(it, g_spawner.m_animals.end());

    for (size_t idxA = 0; idxA < g_players.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < g_players.size(); ++idxB)
            DinoEntity::ResolveCollision(g_players[idxA], g_players[idxB]);

    for (size_t idxA = 0; idxA < g_spawner.m_animals.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < g_spawner.m_animals.size(); ++idxB)
            DinoEntity::ResolveCollision(g_spawner.m_animals[idxA], g_spawner.m_animals[idxB]);

    for (size_t idxA = 0; idxA < g_players.size(); ++idxA)
        for (size_t idxB = 0; idxB < g_spawner.m_animals.size(); ++idxB)
            DinoEntity::ResolveCollision(g_players[idxA], g_spawner.m_animals[idxB]);

    if (g_Lassos.size() != g_players.size())
        DINO_CRITICAL("Il devrait y avoir autant de lassos que de joueurs");
    for (int i = 0; i < g_Lassos.size(); ++i)
        g_Lassos[i].Update(g_players[i].GetPos());

    for (size_t idxA = 0; idxA < g_Lassos.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < g_Lassos.size(); ++idxB)
            DinoLasso::ResolveCollision(g_Lassos[idxA], g_Lassos[idxB]);

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);
    DinoVec2 renderSize = XDino_GetRenderSize();

    //Affichage de terrain
    g_terrain.Draw();
    g_terrain.Update(timeSinceStart);
    for (DinoLasso& lasso : g_Lassos)
        lasso.Draw();

    /*
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", 0.0042 * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }
    */

    // Affichage du nom
    {
        XDino_Draw(vbuffID_nom,
                   XDino_TEXID_FONT,
                   {renderSize.x - text_Size_nom.x * 2, renderSize.y - text_Size_nom.y * 2},
                   2);
    }

    // Affichage du timer
    {

        std::string text = std::format("{:2.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Chrono");
        float tx = (RENDER_SIZE.x - textSize.x * 2) / 2;
        XDino_Draw(vbufID,
                   XDino_TEXID_FONT,
                   {tx, 0},
                   2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    g_spawner.Update(deltaTime, timeSinceStart, g_chrono);

    std::vector<DinoEntity*> entities;
    for (DinoPlayer& player : g_players) {
        entities.push_back(&player);
    }
    for (DinoAnimal& animal : g_spawner.m_animals) {
        entities.push_back(&animal);
    }

    for (DinoLasso& lasso : g_Lassos)
        for (DinoEntity* pEntity : entities)
            if (lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart);

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    g_chrono -= deltaTime;

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);

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
    for (DinoPlayer& player : g_players) {
        player.Shut();
    }
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyVertexBuffer(vbuffID_nom);
    g_spawner.Shut();
    g_terrain.Shut();
}