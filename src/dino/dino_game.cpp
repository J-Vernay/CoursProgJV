/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_player.h"

#include <algorithm>
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double scale = 1.0;

std::vector<DinoPlayer> g_players;

constexpr DinoVec2 RENDER_SIZE = {480, 360};

void InitDinoPlayers();
void UpdateDinoPlayers(float deltaTime);
void DrawDinoPlayers(double timeSinceStart);
void DrawTerrain();

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(RENDER_SIZE);

    InitDinoPlayers();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.
    UpdateDinoPlayers(deltaTime);

    // Affichage
    DinoVec2 windowSize = XDino_GetWindowSize();
    DinoVec2 renderSize = XDino_GetRenderSize();
    // XDino_SetRenderSize(windowSize);

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    DrawTerrain();

    // Dessin du player que l'on peut bouger.
    DrawDinoPlayers(timeSinceStart);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Afficher le nom et le prénom en bas à droite de l'écran
    {
        std::string text = "Antoine VOLLET";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        drawCall.translation.x = windowSize.x - textSize.x * 2;
        drawCall.translation.y = windowSize.y - textSize.y * 2;
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{
    // noop
}

DinoVec2 GetSpawnPos(int playerIndex, DinoVec2 windowSize)
{
    switch (playerIndex) {
    case 0: return {windowSize.x * 0.25f, windowSize.y * 0.25f};
    case 1: return {windowSize.x * 0.75f, windowSize.y * 0.25f};
    case 2: return {windowSize.x * 0.25f, windowSize.y * 0.75f};
    case 3: return {windowSize.x * 0.75f, windowSize.y * 0.75f};
    default: return {windowSize.x * 0.5f, windowSize.y * 0.5f};
    }
}

void InitDinoPlayers()
{
    g_players = std::vector<DinoPlayer>(4);
    for (int i = 0; i < g_players.size(); i++) {
        DinoPlayer player = DinoPlayer();
        player.Init(static_cast<DinoGamepadIdx>(i), GetSpawnPos(i, RENDER_SIZE));
        g_players[i] = player;
    }
}

void UpdateDinoPlayers(float deltaTime)
{
    for (DinoPlayer& player : g_players) {
        player.Update(deltaTime);
    }
}

void DrawDinoPlayers(double timeSinceStart)
{
    std::sort(g_players.begin(), g_players.end(), DinoPlayer::DinoPlayerCompare);

    for (DinoPlayer& player : g_players) {
        player.Draw(timeSinceStart);
    }
}

void DrawTerrain()
{
    DinoVec2 renderSize = XDino_GetRenderSize();

    DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite("terrain.png", renderSize.x, renderSize.y, 0, 0, 16, 16);
    XDino_Draw(drawCall);
}