/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> g_Players;
DinoAnimal g_Animal;

bool ComparePlayersPos(DinoPlayer& a, DinoPlayer& b)
{
    return a.IsAbove(b);
}

void Dino_GameInit()
{
    DinoVec2 renderSize = {480,360};
    XDino_SetRenderSize(renderSize);
    g_Players.resize(4);
    g_Players[0].Init({renderSize.x / 2 - 50, renderSize.y / 2 - 50}, 0, DinoGamepadIdx::Keyboard);
    g_Players[1].Init({renderSize.x / 2 - 50, renderSize.y / 2 + 50}, 1, DinoGamepadIdx::Gamepad1);
    g_Players[2].Init({renderSize.x / 2 + 50, renderSize.y / 2 - 50}, 2, DinoGamepadIdx::Gamepad2);
    g_Players[3].Init({renderSize.x / 2 + 50, renderSize.y / 2 + 50}, 3, DinoGamepadIdx::Gamepad3);

    g_Animal.Init({renderSize.x / 2, renderSize.y / 2}, XDino_RandomInt32(0, 7));
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

	for (DinoPlayer& player : g_Players)
		player.UpdatePlayer(deltaTime);
    g_Animal.UpdateAnimal(deltaTime);
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    DinoVec2 renderSize = XDino_GetRenderSize();
    XDino_SetClearColor(CLEAR_COLOR);

    {
        DinoVec2 terrainSize = { 256, 192};
        DinoDrawCall drawCall;
        drawCall.textureName = "terrain.png";
        
        DinoVec2 posA, posB, posC, posD;
        posA.x = (renderSize.x - terrainSize.x) / 2;
        posA.y = (renderSize.y - terrainSize.y) / 2;
        posB.x = posA.x + terrainSize.x;
        posB.y = posA.y;
        posC.x = posA.x;
        posC.y = posA.y + terrainSize.y;
        posD.x = posA.x + terrainSize.x;
        posD.y = posA.y + terrainSize.y;

        // Océan en fond
        drawCall.vertices.emplace_back(DinoVec2{0,0}, 0, 0);
        drawCall.vertices.emplace_back(DinoVec2{renderSize.x,0}, 16, 0);
        drawCall.vertices.emplace_back(DinoVec2{0,renderSize.y}, 0, 16);
        drawCall.vertices.emplace_back(DinoVec2{renderSize.x,0}, 16, 0);
        drawCall.vertices.emplace_back(DinoVec2{0,renderSize.y}, 0, 16);
        drawCall.vertices.emplace_back(DinoVec2{renderSize.x,renderSize.y}, 16, 16);

        // Terrain au milieu
        drawCall.vertices.emplace_back(posA, 16, 0);
        drawCall.vertices.emplace_back(posB, 32, 0);
        drawCall.vertices.emplace_back(posC, 16, 16);
        drawCall.vertices.emplace_back(posB, 32, 0);
        drawCall.vertices.emplace_back(posC, 16, 16);
        drawCall.vertices.emplace_back(posD, 32, 16);

        XDino_Draw(drawCall);
    }

    std::sort(g_Players.begin(), g_Players.end(), ComparePlayersPos);
    for (DinoPlayer& player : g_Players)
        player.DrawPlayer(timeSinceStart);
    
    g_Animal.DrawAnimal(timeSinceStart);
    
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
        drawCall.translation.x = renderSize.x - 2 * textSize.x;
        drawCall.translation.y = renderSize.y - 2 * textSize.y;
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}