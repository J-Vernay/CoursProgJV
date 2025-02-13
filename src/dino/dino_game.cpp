/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>

#include <algorithm>
#include <vector>
#include <format>

// Variables globales.
double lastTime = 0;
double scale = 1.0;
std::vector<dino_animal> g_animal;
std::vector<dino_player> players;

bool CompareDinoPlayers(dino_player& a, dino_player& b)
{
    return a.IsAbove(b);
}

void Dino_GameInit()
{
    DinoVec2 renderSize = {480, 360};
    XDino_SetRenderSize(renderSize);
    players.resize(4);
    players[0].InitDino({100, 100}, 0, DinoGamepadIdx::Keyboard);
    players[1].InitDino({200, 100}, 1, DinoGamepadIdx::Gamepad1);
    players[2].InitDino({300, 100}, 2, DinoGamepadIdx::Gamepad2);
    players[3].InitDino({400, 100}, 3, DinoGamepadIdx::Gamepad3);

}

double g_AnimalLastSpawnTime = 0;

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    for (dino_player& player : players) {
        player.UpdatePlayer(deltaTime);
    }

    double timeSinceLastSpawn = timeSinceStart - g_AnimalLastSpawnTime;
    if (timeSinceLastSpawn > 1) {
        g_AnimalLastSpawnTime = timeSinceStart;
        g_animal.push_back(dino_animal());
        g_animal.back().InitAnimal({XDino_RandomFloat(0, 256), XDino_RandomFloat(0, 192)}, 0);
    }
    for (dino_animal& animal : g_animal) {
        animal.UpdateAnimal(deltaTime);
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    DinoVec2 renderSize = XDino_GetRenderSize();
    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    XDino_SetRenderSize(renderSize);

    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Rectangle({renderSize.x, renderSize.y}, DinoColor_WHITE);
        XDino_Draw(drawCall);
        DinoDrawCall drawCallTerrain = Dino_CreateDrawCall_Terrain({256, 192});
        drawCallTerrain.translation = {(renderSize.x - 256) / 2, (renderSize.y - 192) / 2};
        XDino_Draw(drawCallTerrain);
    }

    std::sort(players.begin(), players.end(), CompareDinoPlayers);
    for (dino_player& player : players) {
        player.DrawDino(timeSinceStart);
    }

    for (dino_animal& animal : g_animal) {
        animal.DrawAnimal(timeSinceStart);
    }

    // Clément
    {
        std::string text = std::format("LI Clement");
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        // translation en bas à droite
        drawCall.translation = {renderSize.x - textSize.x * 2, renderSize.y - textSize.y * 2};
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}