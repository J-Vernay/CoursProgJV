/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_tree.h"

#include <algorithm>
#include <dino/dino_terrain.h>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <iostream>

#include <format>
#include <map>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

struct PlayerState {
    DinoGamepadIdx gamepadIdx;
    DinoGamepad gamepad;
    DinoPlayer player;
    DinoLasso lasso;
    int score;
};

std::vector<int> g_FreePlayerIndices = {0, 1, 2, 3};
constexpr DinoColor PLAYER_COLORS[4] = {
    DinoColor_BLUE,
    DinoColor_RED,
    DinoColor_YELLOW,
    DinoColor_GREEN,
};
std::vector<DinoGamepadIdx> g_UnassignedGamepads;
std::vector<DinoGamepadIdx> g_AssignedGamepads;
std::vector<PlayerState> g_players;
std::vector<DinoTree> g_Trees;

DinoTerrain g_terrain;
DinoAnimalSpawner g_spawner;


double g_chrono = 60;
bool g_paused = false;
bool g_InLobby = true;
bool g_bWasStartPressed = false;

uint64_t vbuffID_nom;
DinoVec2 text_Size_nom;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr DinoVec2 RENDER_SIZE = {480, 360};

void Initialize_Lobby()
{
    g_InLobby = true;
};

void Dino_GameInit()
{
    DinoPlayer::InitStatic();
    DinoTree::InitStatic();
    g_spawner.Init();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        g_UnassignedGamepads.emplace_back(idx);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_terrain.Init(RENDER_SIZE, idxSeason);

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    // Préparation du drawCall du nom
    {
        std::vector<DinoVertex> vs;
        text_Size_nom = Dino_GenVertices_Text(vs, "Betremieux Elouan", DinoColor_WHITE, DinoColor_GREY);
        vbuffID_nom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

    DinoVec2 terrainMin = g_terrain.GetTopLeft();
    DinoVec2 terrainMax = g_terrain.GetBottomRight();
    for (int i = 0; i < 4; ++i) {
        float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
        float y = terrainMin.y + 80;
        g_Trees.emplace_back(DinoVec2{x, y}, i);
    }

}


void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    XDino_SetRenderSize({480, 360});

    if (g_InLobby) {
        for (int i = 0; i < g_UnassignedGamepads.size(); i++) {
            DinoGamepadIdx idx = g_UnassignedGamepads[i];
            DinoGamepad gamepad;
            if (XDino_GetGamepad(idx, gamepad)) {
                if (gamepad.start) {
                    if (!g_FreePlayerIndices.empty()) {
                        int idxPlayer = g_FreePlayerIndices.front();
                        g_FreePlayerIndices.erase(g_FreePlayerIndices.begin());
                        g_players.emplace_back(idx, gamepad, idxPlayer, DinoLasso(PLAYER_COLORS[idxPlayer], idxPlayer));
                        g_AssignedGamepads.emplace_back(idx);
                        g_UnassignedGamepads.erase(g_UnassignedGamepads.begin() + i);
                    }
                    break;
                }

            }

        }

        for (int i = 0; i < g_AssignedGamepads.size(); i++) {
            DinoGamepadIdx idx = g_AssignedGamepads[i];
            DinoGamepad gamepad;
            if (XDino_GetGamepad(idx, gamepad)) {
                if (gamepad.select) {
                    auto it = std::find_if(g_players.begin(),
                                           g_players.end(),
                                           [idx](PlayerState& ps) {
                                               return ps.gamepadIdx == idx;
                                           });
                    if (it != g_players.end()) {
                        g_FreePlayerIndices.push_back(it->player.m_idxPlayer);
                        std::sort(g_FreePlayerIndices.begin(), g_FreePlayerIndices.end());
                        g_players.erase(it);
                    }

                    g_UnassignedGamepads.emplace_back(idx);
                    g_AssignedGamepads.erase(g_AssignedGamepads.begin() + i);
                    break;
                }

            }

        }
    }

    bool bPressedStart = false;
    for (PlayerState& player : g_players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad)) {
            player.gamepad = gamepad;
            bPressedStart = bPressedStart || gamepad.start;
        }
    }

    // Mettre en pause le jeu
    if (!g_InLobby) {
        if (bPressedStart && !g_bWasStartPressed)
            g_paused = !g_paused;
        g_bWasStartPressed = bPressedStart;
    }

    if (!g_paused) {
        for (PlayerState& player : g_players)
            player.player.Update(timeSinceStart, deltaTime, g_terrain, player.gamepad);
    }

    // TO REMOVE
    if (g_players.size() > 0) {
        if (g_players[0].gamepad.btn_up) {
            g_players[0].score += 10;
        }

    }

    if (!g_paused && !g_InLobby) {
        auto it = std::remove_if(g_spawner.m_animals.begin(), g_spawner.m_animals.end(), DinoAnimal::IsDead);
        for (auto it2 = it; it2 < g_spawner.m_animals.end(); ++it2)
            it2->Shut();

        g_spawner.m_animals.erase(it, g_spawner.m_animals.end());

        g_spawner.Update(deltaTime, timeSinceStart, g_chrono);
    }

    std::vector<DinoEntity*> entities;
    for (PlayerState& player : g_players) {
        entities.push_back(&player.player);
    }
    for (DinoAnimal& animal : g_spawner.m_animals) {
        entities.push_back(&animal);
    }
    if (g_InLobby) {
        for (DinoTree& tree : g_Trees)
            entities.emplace_back(&tree);
    }

    if (!g_paused) {

        for (size_t idxA = 0; idxA < entities.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB) {
                DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);
            }

        for (PlayerState& player : g_players)
            player.lasso.Update(player.player.GetPos());

        for (size_t idxA = 0; idxA < g_players.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < g_players.size(); ++idxB)
                DinoLasso::ResolveCollision(g_players[idxA].lasso, g_players[idxB].lasso);

        for (PlayerState& player : g_players)
            for (DinoEntity* pEntity : entities)
                if (player.lasso.WasInLoop(pEntity->GetPos()))
                    pEntity->ReactLoop(timeSinceStart, player.lasso.m_ownerIndex);
    }

    if (g_InLobby) {
        for (DinoTree& tree : g_Trees)
            if (tree.WasLooped()) {
                g_InLobby = false;
                g_terrain.Shut();
                g_terrain.Init(RENDER_SIZE, tree.GetIdxSeason());
                g_Trees.clear();
                break;
            }
    }

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Affichage
    DinoVec2 renderSize = XDino_GetRenderSize();

    //Affichage de terrain
    g_terrain.Draw();
    g_terrain.Update(timeSinceStart);
    for (PlayerState& player : g_players)
        player.lasso.Draw();

    // Affichage du nom
    {
        XDino_Draw(vbuffID_nom,
                   XDino_TEXID_FONT,
                   {renderSize.x - text_Size_nom.x * 2, renderSize.y - text_Size_nom.y * 2},
                   2);
    }

    // Affichage du timer
    if (!g_InLobby) {

        std::string text = std::format("{:2.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vertex_buffer = {vs.data(), vs.size(), "Chrono"};
        float tx = (RENDER_SIZE.x - textSize.x * 2) / 2;
        XDino_Draw(vertex_buffer.GetVbufID(),
                   XDino_TEXID_FONT,
                   {tx, 0},
                   2);
    }
    else {
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, "Choose a tree to start", DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vertex_buffer = {vs.data(), vs.size(), "Title"};
        float tx = (RENDER_SIZE.x - textSize.x * 2) / 2;
        XDino_Draw(vertex_buffer.GetVbufID(),
                   XDino_TEXID_FONT,
                   {tx, 0},
                   2);

        for (size_t i = 0; i < g_players.size(); ++i) {
            PlayerState& player = g_players[i];
            vs.clear();
            std::string text = " Player " + std::to_string(i + 1) + " \n " + std::to_string(g_players[i].score);
            textSize = Dino_GenVertices_Text(
                vs,
                text,
                PLAYER_COLORS[player.player.m_idxPlayer],
                DinoColor_BLACK
            );

            DinoVertexBuffer new_vertex_buffer = {vs.data(), vs.size(), "PlayerScore"};

            DinoVec2 topLeft = g_terrain.GetTopLeft();
            DinoVec2 bottomRight = g_terrain.GetBottomRight();
            float ty = bottomRight.y / 4;
            float x = 0;
            float y = topLeft.y + i * ty;

            XDino_Draw(new_vertex_buffer.GetVbufID(),
                       XDino_TEXID_FONT,
                       {x, y},
                       1.2f);
        }

    }

    if (!g_paused && !g_InLobby) {
        g_chrono -= deltaTime;

    }

    if (g_chrono <= 0) {
        g_InLobby = true;
        DinoVec2 terrainMin = g_terrain.GetTopLeft();
        DinoVec2 terrainMax = g_terrain.GetBottomRight();
        for (int i = 0; i < 4; ++i) {
            float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
            float y = terrainMin.y + 80;
            g_Trees.emplace_back(DinoVec2{x, y}, i);
        }
        g_chrono = 60;
        g_spawner.m_animals.clear();
    }

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);

    if (g_paused) {
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, "-- PAUSE --", DinoColor_WHITE, DinoColor_BLACK);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (RENDER_SIZE.x - textSize.x * 6) / 2;
        float ty = (RENDER_SIZE.y - textSize.y * 6) / 2;
        XDino_Draw(vbuf.GetVbufID(), XDino_TEXID_FONT, {tx, ty}, 6);
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
    for (PlayerState& player : g_players) {
        player.player.Shut();
    }
    XDino_DestroyVertexBuffer(vbuffID_nom);
    g_spawner.Shut();
    g_terrain.Shut();
}