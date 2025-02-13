/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include "dino/dino_player.h"

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
std::vector<DinoVec2> polyline;


DinoPlayer player;
DinoPlayer player1;
DinoPlayer player2;
DinoPlayer player3;


// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    player.Init({windowSize.x / 2, windowSize.y / 2});
    player1.Init({windowSize.x / 2 - 100, windowSize.y / 2});
    player2.Init({windowSize.x / 2, windowSize.y / 2 - 100});
    player3.Init({windowSize.x / 2 - 100, windowSize.y / 2 - 100});
}


void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    player.UpdatePlayer(static_cast<float>(deltaTime));
    player1.UpdatePlayer(static_cast<float>(deltaTime));
    player2.UpdatePlayer(static_cast<float>(deltaTime));
    player3.UpdatePlayer(static_cast<float>(deltaTime));

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne" 
    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(polyline, 100, POLYLINE_COLOR);
        XDino_Draw(drawCall);
    }

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    player.DrawPlayer(static_cast<float>(timeSinceStart));
    player1.DrawPlayer(static_cast<float>(timeSinceStart));
    player2.DrawPlayer(static_cast<float>(timeSinceStart));
    player3.DrawPlayer(static_cast<float>(timeSinceStart));

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
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_RED);
        drawCall.scale = 1;
        drawCall.rotation = 180;
        drawCall.translation = {windowSize.x, windowSize.y};
        XDino_Draw(drawCall);
    }

}

void Dino_GameShut()
{

}