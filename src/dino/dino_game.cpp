/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_entity.h"

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>
#include <dino/dino_terrain.h>
#include <dino/dino_geometry.h>

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<dino_player> dinoPlayers;
std::vector<dino_animal> animals;

dino_terrain terrain;

bool CompareEntitiesPos(dino_entity* pEntity1, dino_entity* pEntity2)
{
    return pEntity1->GetPos().y < pEntity2->GetPos().y;
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

    double timeSinceLastSPawn = timeSinceStart - animalLastSpawnTime;
    if (timeSinceLastSPawn > 1) {
        animalLastSpawnTime = timeSinceStart;
        animals.emplace_back();
        animals.back().Init({renderSize.x / 2, renderSize.y / 2}, XDino_RandomInt32(0, 7));
    }

    std::vector<dino_entity*> pEntities;
    for (dino_player& player : dinoPlayers)
        pEntities.emplace_back(&player);
    for (dino_animal& animal : animals)
        pEntities.emplace_back(&animal);

    for (dino_entity* pEntity : pEntities)
        pEntity->Update(deltaTime);

    // Gérer les collisions entre joueurs.
    for (dino_entity* pEntity1 : pEntities) {
        for (dino_entity* pEntity2 : pEntities) {
            DinoVec2 a = pEntity1->GetPos();
            DinoVec2 b = pEntity2->GetPos();
            Dino_ResolveCircleCollision(a, b, 8);
            pEntity1->SetPos(a);
            pEntity2->SetPos(b);
        }
    }

    for (dino_entity* pEntity : pEntities) {
        pEntity->ApplyTerain(terrainA, terrainB);
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    terrain.DrawTerrain(renderSize, terrainSize);

    std::sort(pEntities.begin(), pEntities.end(), CompareEntitiesPos);
    for (dino_entity* pEntity : pEntities)
        pEntity->Draw(timeSinceStart);

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