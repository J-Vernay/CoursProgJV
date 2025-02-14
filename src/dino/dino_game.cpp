/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_geometry.h>

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> g_Players;
std::vector<DinoAnimal> g_Animals;

bool CompareEntitiesPos(DinoEntity* pEntity1, DinoEntity* pEntity2)
{
    return pEntity1->GetPos().y < pEntity2->GetPos().y;
}

void Dino_GameInit()
{
    DinoVec2 renderSize = {480,360};
    XDino_SetRenderSize(renderSize);
    g_Players.resize(4);
    g_Players[0].Init({renderSize.x / 2 - 50, renderSize.y / 2 - 50}, 0, DinoGamepadIdx::Keyboard, DinoColor_BLUE);
    g_Players[1].Init({renderSize.x / 2 - 50, renderSize.y / 2 + 50}, 1, DinoGamepadIdx::Gamepad1, DinoColor_RED);
    g_Players[2].Init({renderSize.x / 2 + 50, renderSize.y / 2 - 50}, 2, DinoGamepadIdx::Gamepad2, DinoColor_YELLOW);
    g_Players[3].Init({renderSize.x / 2 + 50, renderSize.y / 2 + 50}, 3, DinoGamepadIdx::Gamepad3, DinoColor_GREEN);

}

double g_AnimalLastSpawnTime = 0;

void Dino_GameFrame(double timeSinceStart)
{
    DinoVec2 renderSize = XDino_GetRenderSize();
    
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Terrain
    
    DinoVec2 terrainSize = { 256, 192};
    DinoVec2 terrainA, terrainB;
    terrainA.x = (renderSize.x - terrainSize.x) / 2;
    terrainA.y = (renderSize.y - terrainSize.y) / 2;
    terrainB.x = terrainA.x + terrainSize.x;
    terrainB.y = terrainA.y + terrainSize.y;

    double timeSinceLastSpawn = timeSinceStart - g_AnimalLastSpawnTime;
    if (timeSinceLastSpawn > 0.1) {
        g_AnimalLastSpawnTime = timeSinceStart;
        g_Animals.emplace_back();
        g_Animals.back().Init({renderSize.x / 2, renderSize.y / 2}, XDino_RandomInt32(0, 7));
    }

    std::vector<DinoEntity*> pEntities;
    for (DinoPlayer& player : g_Players)
        pEntities.emplace_back(&player);
    for (DinoAnimal& animal : g_Animals)
        pEntities.emplace_back(&animal);

    for (DinoEntity* pEntity : pEntities)
        pEntity->Update(deltaTime);
    
    // Gérer les collisions entre animaux.
    for (DinoEntity* pEntity1 : pEntities) {
        for (DinoEntity* pEntity2 : pEntities) {
            DinoVec2 a = pEntity1->GetPos();
            DinoVec2 b = pEntity2->GetPos();
            Dino_ResolveCircleCollision(a, b, 8);
            pEntity1->SetPos(a);
            pEntity2->SetPos(b);
        }
    }
    
    for (DinoEntity* pEntity : pEntities)
        pEntity->ApplyTerrain(terrainA, terrainB);
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    {
        DinoDrawCall drawCall;
        drawCall.textureName = "terrain.png";
        
        DinoVec2 posA, posB, posC, posD;
        posA = terrainA;
        posB.x = posA.x + terrainSize.x;
        posB.y = posA.y;
        posC.x = posA.x;
        posC.y = posA.y + terrainSize.y;
        posD = terrainB;
        
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

    for (DinoPlayer& player : g_Players)
        player.DrawLasso();

    std::sort(pEntities.begin(), pEntities.end(), CompareEntitiesPos);
    for (DinoEntity* pEntity : pEntities)
        pEntity->Draw(timeSinceStart);
    
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        int sizeVec2 = sizeof(DinoVec2);
        int sizeColor = sizeof(DinoColor);
        int sizeVertex = sizeof(DinoVertex);
        std::string text = std::format("sizeVec2={}  sizeColor={}  sizeVertex={}", sizeVec2, sizeColor, sizeVertex);
        //std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
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