/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.


#include "dino_animal.h"
#include <algorithm>
#include <dino/dino_player.h>
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <format>

// Variables globales.
double lastTime = 0;


std::vector<DinoPlayer> g_Players;
DinoAnimal g_Animal;

bool ComparePlayerPos(DinoPlayer& a, DinoPlayer& b)
{
    return a.IsAbove(b);
}

void RenderTerrain(DinoVec2 renderSize)
{
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
    
}


void Dino_GameInit()
{
    DinoVec2 renderSize = {480,360};
    XDino_SetRenderSize(renderSize);
    g_Players.resize(4);
    g_Players[0].Init({renderSize.x / 2 - 100, renderSize.y / 2 - 100}, 0, DinoGamepadIdx::Keyboard);
    g_Players[1].Init({renderSize.x / 2 - 100, renderSize.y / 2 + 100}, 1, DinoGamepadIdx::Gamepad1);
    g_Players[2].Init({renderSize.x / 2 + 100, renderSize.y / 2 - 100}, 2, DinoGamepadIdx::Gamepad2);
    g_Players[3].Init({renderSize.x / 2 + 100, renderSize.y / 2 + 100}, 3, DinoGamepadIdx::Gamepad3);

    g_Animal.Init({renderSize.x / 2, renderSize.y / 2}, XDino_RandomInt32(0, 7));
}


void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    for (DinoPlayer& player : g_Players) {
        player.UpdatePlayer(deltaTime);
    }
    

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    DinoVec2 renderSize = XDino_GetWindowSize();
    XDino_SetRenderSize(renderSize);

    RenderTerrain(renderSize);

    std::sort(g_Players.begin(), g_Players.end(), ComparePlayerPos);
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
        std::string text = "Thomas Druesne";
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


/*// Dessin de la texture centrale qu'on peut bouger.
    {
        constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

        float quarterWidth = windowSize.x / 4;
        float quarterHeight = windowSize.y / 4;

        DinoDrawCall drawCall;
        drawCall.vertices.resize(6);
        drawCall.vertices[0].pos = {-quarterWidth, -quarterHeight};
        drawCall.vertices[0].color = PURPLE;
        drawCall.vertices[1].pos = {quarterWidth, -quarterHeight};
        drawCall.vertices[1].color = CYAN;
        drawCall.vertices[2].pos = {-quarterWidth, quarterHeight};
        drawCall.vertices[2].color = PINK;
        drawCall.vertices[3].pos = {quarterWidth, -quarterHeight};
        drawCall.vertices[3].color = CYAN;
        drawCall.vertices[4].pos = {-quarterWidth, quarterHeight};
        drawCall.vertices[4].color = PINK;
        drawCall.vertices[5].pos = {quarterWidth, quarterHeight};
        drawCall.vertices[5].color = ORANGE;

        drawCall.translation = {windowSize.x / 2, windowSize.y / 2};
        drawCall.rotation = rotation;
        drawCall.scale = scale;

        drawCall.vertices[0].u = 0;
        drawCall.vertices[0].v = 0;
        drawCall.vertices[1].u = 96;
        drawCall.vertices[1].v = 0;
        drawCall.vertices[2].u = 0;
        drawCall.vertices[2].v = 96;
        drawCall.vertices[3].u = 96;
        drawCall.vertices[3].v = 0;
        drawCall.vertices[4].u = 0;
        drawCall.vertices[4].v = 96;
        drawCall.vertices[5].u = 96;
        drawCall.vertices[5].v = 96;
        drawCall.textureName = "monogram-bitmap.png";

        XDino_Draw(drawCall);
    }*/