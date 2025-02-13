/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> g_Players;

bool ComparePlayersPos(DinoPlayer& a, DinoPlayer& b)
{
    return a.IsAbove(b);
}

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_Players.resize(4);
    g_Players[0].Init({windowSize.x / 2 - 100, windowSize.y / 2 - 100}, 0, DinoGamepadIdx::Keyboard);
    g_Players[1].Init({windowSize.x / 2 - 100, windowSize.y / 2 + 100}, 1, DinoGamepadIdx::Gamepad1);
    g_Players[2].Init({windowSize.x / 2 + 100, windowSize.y / 2 - 100}, 2, DinoGamepadIdx::Gamepad2);
    g_Players[3].Init({windowSize.x / 2 + 100, windowSize.y / 2 + 100}, 3, DinoGamepadIdx::Gamepad3);
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

	for (DinoPlayer& player : g_Players)
		player.UpdatePlayer(deltaTime);
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    std::sort(g_Players.begin(), g_Players.end(), ComparePlayersPos);
    for (DinoPlayer& player : g_Players)
        player.DrawPlayer(timeSinceStart);
    
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Prénom.
    {
        std::string text = "Julien Vernay";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.translation.x = windowSize.x - 2 * textSize.x;
        drawCall.translation.y = windowSize.y - 2 * textSize.y;
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}