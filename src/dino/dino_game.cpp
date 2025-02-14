/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>
#include <dino/dino_terrain.h>

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<dino_player> dinoPlayers;
std::vector<dino_animal> animals;

dino_terrain terrain;

bool ComparePlayerPos(dino_player& a, dino_player& b)
{
    return a.IsAbove(b);
}

bool CompareAnimalPos(dino_animal& a, dino_animal& b)
{
    return a.IsAbove(b);
}

void Dino_GameInit()
{
    DinoVec2 renderSize = {480, 360};
    XDino_SetRenderSize(renderSize);
    dinoPlayers.resize(4);
    dinoPlayers[0].Init({renderSize.x / 2 - 50, renderSize.y / 2 - 50}, 0, DinoGamepadIdx::Gamepad1);
    dinoPlayers[1].Init({renderSize.x / 2 - 50, renderSize.y / 2 + 50}, 1, DinoGamepadIdx::Gamepad2);
    dinoPlayers[2].Init({renderSize.x / 2 + 50, renderSize.y / 2 - 50}, 2, DinoGamepadIdx::Gamepad3);
    dinoPlayers[3].Init({renderSize.x / 2 + 50, renderSize.y / 2 + 50}, 3, DinoGamepadIdx::Gamepad4);
}

double animalLastSpawnTime = 0;

void Dino_GameFrame(double timeSinceStart)
{
    DinoVec2 renderSize = XDino_GetRenderSize();

    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    //terrain 
    DinoVec2 terrainSize = {256, 192};
    DinoVec2 terrainA, terrainB;
    terrainA.x = (renderSize.x - terrainSize.x) / 2;
    terrainA.y = (renderSize.y - terrainSize.y) / 2;
    terrainB.x = terrainA.x + terrainSize.x;
    terrainB.y = terrainA.y + terrainSize.y;

    for (dino_player& player : dinoPlayers) {
        player.UpdatePlayer(deltaTime);
        player.ApplyTerain(terrainA, terrainB);
    }

    double timeSinceLastSPawn = timeSinceStart - animalLastSpawnTime;
    if (timeSinceLastSPawn > 1) {
        animalLastSpawnTime = timeSinceStart;
        animals.emplace_back();
        animals.back().Init({renderSize.x / 2, renderSize.y / 2}, XDino_RandomInt32(0, 7));
    }

    for (dino_animal& animal : animals) {
        animal.UpdateAnimal(deltaTime);
        animal.ApplyTerain(terrainA, terrainB);
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

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

    std::sort(dinoPlayers.begin(), dinoPlayers.end(), ComparePlayerPos);
    for (dino_player& player : dinoPlayers) {
        player.DrawPlayer(timeSinceStart);
    }

    std::sort(animals.begin(), animals.end(), CompareAnimalPos);
    for (dino_animal& animal : animals) {
        animal.DrawAnimal(timeSinceStart);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // NOM Prénom
    {
        std::string text = "DUFOUR Enzo";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        drawCall.translation.x = renderSize.x - 2 * textSize.x;
        drawCall.translation.y = renderSize.y - 2 * textSize.y;
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}