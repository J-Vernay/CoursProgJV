/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_geometry.h"

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

bool CompareDinoPlayers(DinoEntity* a, DinoEntity* b)
{
    return a->GetPosition().y < b->GetPosition().y;
}

void Dino_GameInit()
{
    DinoVec2 renderSize = {480, 360};
    XDino_SetRenderSize(renderSize);
    players.resize(4);
    players[0].InitDino({100, 100}, 0, DinoGamepadIdx::Keyboard, DinoColor_BLUE);
    players[1].InitDino({200, 100}, 1, DinoGamepadIdx::Gamepad1, DinoColor_RED);
    players[2].InitDino({300, 100}, 2, DinoGamepadIdx::Gamepad2, DinoColor_YELLOW);
    players[3].InitDino({400, 100}, 3, DinoGamepadIdx::Gamepad3, DinoColor_GREEN);
}

double g_AnimalLastSpawnTime = 0;

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    // Terrain

    DinoVec2 size = XDino_GetRenderSize();
    DinoVec2 terrainSize = {256, 192};
    DinoVec2 terrainA, terrainB;
    terrainA.x = (size.x - terrainSize.x) / 2;
    terrainA.y = (size.y - terrainSize.y) / 2;
    terrainB.x = terrainA.x + terrainSize.x;
    terrainB.y = terrainA.y + terrainSize.y;

    double timeSinceLastSpawn = timeSinceStart - g_AnimalLastSpawnTime;
    if (timeSinceLastSpawn > 1) {
        g_AnimalLastSpawnTime = timeSinceStart;
        g_animal.push_back(dino_animal());
        g_animal.back().InitAnimal({size.x / 2, size.y / 2}, XDino_RandomInt32(0, 7));
    }

    // Collision entre joueurs
    std::vector<DinoEntity*> entities;
    for (dino_player& player : players) {
        entities.push_back(&player);
    }

    for (dino_animal& animal : g_animal) {
        entities.push_back(&animal);
    }

    for (DinoEntity* entity : entities) {
        entity->Update(deltaTime);
    }

    for (DinoEntity* entity : entities) {
        for (DinoEntity* other : entities) {
            DinoVec2 pos = entity->GetPosition();
            DinoVec2 otherPos = other->GetPosition();
            ResolveCollision(pos, otherPos, 8);
            entity->SetPosition(pos);
            other->SetPosition(otherPos);
        }
    }

    // Collision entre les bords du terrain
    for (DinoEntity* entity : entities) {
        entity->ApplyTerrain(terrainA, terrainB);
    }
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 renderSize = XDino_GetRenderSize();
    XDino_SetRenderSize(renderSize);

    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Rectangle({renderSize.x, renderSize.y}, DinoColor_WHITE);
        XDino_Draw(drawCall);
        DinoDrawCall drawCallTerrain = Dino_CreateDrawCall_Terrain({256, 192});
        drawCallTerrain.translation = {(renderSize.x - 256) / 2, (renderSize.y - 192) / 2};
        XDino_Draw(drawCallTerrain);
    }

    for (dino_player& player : players) {
        player.DrawLasso();
    }

    std::sort(entities.begin(), entities.end(), CompareDinoPlayers);
    for (DinoEntity* entity : entities) {
        entity->Draw(timeSinceStart);
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