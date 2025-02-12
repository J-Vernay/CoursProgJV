/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>

#include <vector>
#include <format>

// Variables globales.
double lastTime = 0;
double scale = 1.0;
dino_player player1, player2, player3, player4;
std::vector players = {player1, player2, player3, player4};

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    for (int i = 0; i < 4; i++) {
        players[i].InitDino({100 * static_cast<float>(i), 100}, i);
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    for (dino_player& player : players) {
        player.UpdatePlayer(deltaTime);
        player.DrawDino(timeSinceStart);
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    // Clément
    {
        std::string text = std::format("LI Clement");
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        // translation en bas à droite
        drawCall.translation = {windowSize.x - textSize.x * 2, windowSize.y - textSize.y * 2};
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}