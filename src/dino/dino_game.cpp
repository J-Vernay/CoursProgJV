/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_map.h>
#include <dino/dino_animals.h>

#include <format>
#include <algorithm>

const DinoVec2 WINDOW_SIZE = {480, 360};

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
std::vector<DinoVec2> polyline;

DinoMap map;

DinoPlayer player1;
DinoPlayer player2;
DinoPlayer player3;
DinoPlayer player4;
    
std::vector<DinoPlayer> players {player1, player2, player3, player4};
std::vector<DinoAnimal> animals;
void Dino_GameInit()
{
    XDino_SetRenderSize(WINDOW_SIZE);

    players[0].DinoPlayerInit({WINDOW_SIZE.x/2 - 20, WINDOW_SIZE.y/2 + 20}, 0, DinoGamepadIdx::Keyboard);
    players[1].DinoPlayerInit({WINDOW_SIZE.x/2 + 20, WINDOW_SIZE.y/2 + 20}, 1, DinoGamepadIdx::Gamepad1);
    players[2].DinoPlayerInit({WINDOW_SIZE.x/2 - 20, WINDOW_SIZE.y/2 - 20}, 2, DinoGamepadIdx::Gamepad2);
    players[3].DinoPlayerInit({WINDOW_SIZE.x/2 + 20, WINDOW_SIZE.y/2 - 20}, 3, DinoGamepadIdx::Gamepad3);
}

bool ComparePlayersPos(DinoPlayer& a, DinoPlayer& b)
{
    return a.IsAbove(b);
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};


    XDino_SetClearColor(CLEAR_COLOR);
    
    XDino_SetRenderSize(WINDOW_SIZE);

    map.DisplayMap();

    std::sort(players.begin(), players.end(), ComparePlayersPos);
    
    for (DinoPlayer& player : players) {
        player.UpdatePlayer(deltaTime);
        player.DisplayPlayer(timeSinceStart);
    }

    animals.emplace_back();
    animals.back().DinoAnimalInit({WINDOW_SIZE.x/2, WINDOW_SIZE.y/2}, XDino_RandomInt32(0,7));
    for (DinoAnimal& animal : animals) {
        animal.UpdateAnimal(deltaTime);
        animal.DisplayAnimal(timeSinceStart);
    }

    
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        XDino_Draw(drawCall);
    }

    // Name
    {
        std::string text = std::format("Hugo Lesec");
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.translation = {WINDOW_SIZE.x - textSize.x *2, WINDOW_SIZE.y - textSize.y *2};
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
}


void Dino_GameShut()
{

}