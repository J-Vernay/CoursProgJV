/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include "dino/dino_player.h"

#include <format>
#include <algorithm>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
std::vector<DinoPlayer> players;

bool ComparePlayerPos(DinoPlayer& a, DinoPlayer& b)
{
    return a.isAbove(b);
}

void Dino_GameInit()
{
    XDino_SetRenderSize({480,360});
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    players.resize(4);
    players[0].Init({windowSize.x / 2, windowSize.y / 2}, DinoGamepadIdx::Keyboard, 0);
    players[1].Init({windowSize.x / 2 - 100, windowSize.y / 2}, DinoGamepadIdx::Gamepad1, 1);
    players[2].Init({windowSize.x / 2, windowSize.y / 2 - 100}, DinoGamepadIdx::Gamepad2, 2);
    players[3].Init({windowSize.x / 2 - 100, windowSize.y / 2 - 100}, DinoGamepadIdx::Gamepad3, 3);
}


void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    for (DinoPlayer& player : players) {
        player.UpdatePlayer(deltaTime);
    }

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    std::sort(players.begin(), players.end(), ComparePlayerPos);
    for (DinoPlayer& player : players) {
        player.DrawPlayer(timeSinceStart);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", timeSinceStart * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
    {
        std::string text = std::format("FARIN Pix", timeSinceStart * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_BLUE, DinoColor_RED);
        drawCall.scale = 1;
        drawCall.rotation = 0;
        drawCall.translation = {windowSize.x - 60, windowSize.y - 20};
        XDino_Draw(drawCall);
    }

}

void Dino_GameShut()
{

}