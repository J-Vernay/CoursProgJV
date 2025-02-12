/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
std::vector<DinoVec2> polyline;

DinoPlayer player1;
DinoPlayer player2;
DinoPlayer player3;
DinoPlayer player4;
    
std::vector<DinoPlayer> players {player1, player2, player3, player4};

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
    
    players[0].SetPlayerPos({windowSize.x/2 - 100, windowSize.y/2 + 100});
    players[1].SetPlayerPos({windowSize.x/2 + 100, windowSize.y/2 + 100});
    players[2].SetPlayerPos({windowSize.x/2 - 100, windowSize.y/2 - 100});
    players[3].SetPlayerPos({windowSize.x/2 + 100, windowSize.y/2 - 100});
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    for (DinoPlayer& player : players) {
        player.UpdatePlayer(deltaTime);
        player.DisplayPlayer(timeSinceStart);
    }


    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Name
    {
        std::string text = std::format("Hugo Lesec");
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        drawCall.translation = {windowSize.x - textSize.x *2, windowSize.y - textSize.y *2};
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}