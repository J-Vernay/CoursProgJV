/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_animal.h"

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
//DinoVec2 circlePos = {};
//std::vector<DinoVec2> polyline;

std::vector<DinoPlayer> players;
std::vector<DinoAnimal> animals;

// Constantes.
constexpr float Dino_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

bool ComparePlayerPos(DinoPlayer& a, DinoPlayer& b)
{
    //return a.pos.y < b.pos.y;
    return a.IsAbove(b);
}

void Dino_GameInit()
{
    DinoVec2 renderSize = {480, 360};
    XDino_SetRenderSize(renderSize);

    players.resize(4);

    players[0].Init(0, {renderSize.x / 2 - 50, renderSize.y / 2 - 50});
    players[1].Init(1, {renderSize.x / 2 - 50, renderSize.y / 2 + 50});
    players[2].Init(2, {renderSize.x / 2 + 50, renderSize.y / 2 - 50});
    players[3].Init(3, {renderSize.x / 2 + 50, renderSize.y / 2 + 50});

    /*polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);*/
}

double animalLastSpawnTime = 0;

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    DinoVec2 renderSize = {480, 360};

    DinoVec2 terrainSize = {renderSize.x / 2, renderSize.y / 2};
    // Gestion des entrées et mise à jour de la logique de jeu.

    for (DinoPlayer& player : players) {
        player.UpdatePlayer(deltaTime, renderSize, terrainSize);
    }
    double timeSinceLastSpawn = timeSinceStart - animalLastSpawnTime;
    if (timeSinceLastSpawn > 1) {
        animals.emplace_back();
        animals.back().SpawnAnimal(XDino_RandomUint32(0, 7),
                                   renderSize,
                                   terrainSize);
        animalLastSpawnTime = timeSinceStart;
    }

    for (DinoAnimal& animal : animals) {
        animal.UpdateAnimal(deltaTime, renderSize, terrainSize);
    }
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    //constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);

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
    drawCall.vertices.emplace_back(DinoVec2{0, 0}, 0, 0);
    drawCall.vertices.emplace_back(DinoVec2{renderSize.x, 0}, 16, 0);
    drawCall.vertices.emplace_back(DinoVec2{0, renderSize.y}, 0, 16);
    drawCall.vertices.emplace_back(DinoVec2{renderSize.x, 0}, 16, 0);
    drawCall.vertices.emplace_back(DinoVec2{0, renderSize.y}, 0, 16);
    drawCall.vertices.emplace_back(DinoVec2{renderSize.x, renderSize.y}, 16, 16);
    // Terrain au milieu
    drawCall.vertices.emplace_back(posA, 16, 0);
    drawCall.vertices.emplace_back(posB, 32, 0);
    drawCall.vertices.emplace_back(posC, 16, 16);
    drawCall.vertices.emplace_back(posB, 32, 0);
    drawCall.vertices.emplace_back(posC, 16, 16);
    drawCall.vertices.emplace_back(posD, 32, 16);
    XDino_Draw(drawCall);

    std::sort(players.begin(), players.end(), ComparePlayerPos);

    for (DinoPlayer& player : players) {
        player.DrawPlayer(timeSinceStart);
    }

    for (DinoAnimal& animal : animals) {
        animal.DrawAnimal(timeSinceStart);
    }

    XDino_SetRenderSize(renderSize);

    /* Dessin de la "polyligne" 
    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(polyline, 100, POLYLINE_COLOR);
        XDino_Draw(drawCall);
    }

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    

    // Dessin de la texture centrale qu'on peut bouger.
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
    }

    // Dessin du cercle que l'on peut bouger.
    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Circle(20);
        drawCall.translation = circlePos;
        XDino_Draw(drawCall);
    }*/

    // Dessin du dino

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
    // nom et prénom
    {
        std::string text = "Pharrell Bastien";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.translation.x = renderSize.x - 2 * textSize.x;
        drawCall.translation.y = renderSize.y - 2 * textSize.y;
        drawCall.scale = 2;
        drawCall.translation = {renderSize.x - textSize.x * 2, renderSize.y - textSize.y * 2};

        XDino_Draw(drawCall);
    }

}

void Dino_GameShut()
{

}