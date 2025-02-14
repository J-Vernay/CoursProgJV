/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_animals.h>
#include <dino/dino_geometry.h>

#include <format>
#include <algorithm>

const DinoVec2 WINDOW_SIZE = {480, 360};

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> players;
std::vector<DinoAnimal> animals;

bool CompareEntity(DinoEntity* a, DinoEntity* b)
{
    return a->GetPos().y < b->GetPos().y;
}

void Dino_GameInit()
{
    DinoVec2 renderSize = WINDOW_SIZE;
    XDino_SetRenderSize(renderSize);

    players.resize(4);
    players[0].DinoPlayerInit({WINDOW_SIZE.x/2 - 20, WINDOW_SIZE.y/2 + 20}, 0, DinoGamepadIdx::Keyboard, DinoColor_BLUE);
    players[1].DinoPlayerInit({WINDOW_SIZE.x/2 + 20, WINDOW_SIZE.y/2 + 20}, 1, DinoGamepadIdx::Gamepad1, DinoColor_RED);
    players[2].DinoPlayerInit({WINDOW_SIZE.x/2 - 20, WINDOW_SIZE.y/2 - 20}, 2, DinoGamepadIdx::Gamepad2, DinoColor_YELLOW);
    players[3].DinoPlayerInit({WINDOW_SIZE.x/2 + 20, WINDOW_SIZE.y/2 - 20}, 3, DinoGamepadIdx::Gamepad3, DinoColor_GREEN);
}

double g_AnimalLastSpawnTime = 0;

void Dino_GameFrame(double timeSinceStart)
{
    XDino_ProfileBegin(DinoColor_BLACK, "GameFrame");
    DinoVec2 renderSize = XDino_GetRenderSize();
    
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;
    

    DinoVec2 terrainSize = { 256, 192};
    DinoVec2 terrainA, terrainB;
    terrainA.x = (renderSize.x - terrainSize.x) / 2;
    terrainA.y = (renderSize.y - terrainSize.y) / 2;
    terrainB.x = terrainA.x + terrainSize.x;
    terrainB.y = terrainA.y + terrainSize.y;    

    double timeSinceLastSpawn = timeSinceStart - g_AnimalLastSpawnTime;
    if (timeSinceLastSpawn > 3) {
        g_AnimalLastSpawnTime = timeSinceStart;
        animals.emplace_back();
        animals.back().DinoAnimalInit({renderSize.x / 2, renderSize.y / 2}, XDino_RandomInt32(0, 7));
    }

    std::vector<DinoEntity*> entities;
    for (DinoPlayer& player : players) 
        entities.emplace_back(&player);
    for (DinoAnimal& animal : animals) 
        entities.emplace_back(&animal);

    XDino_ProfileBegin(DinoColor_RED, "Collision");
    for (DinoEntity* entity1 : entities) {
        for (DinoEntity* entity2 : entities) {
            DinoVec2 a = entity1->GetPos();
            DinoVec2 b = entity2->GetPos();
            Dino_ResolveCircleCollision(a, b, 8);
            entity1->SetPos(a);
            entity2->SetPos(b);
        }
    }
    XDino_ProfileEnd();
    
    for (DinoEntity* entity : entities) {
        entity->Update(deltaTime);
        entity->ApplyTerrain(terrainA, terrainB);
    }
    XDino_ProfileEnd();
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

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

    XDino_ProfileBegin(DinoColor_BLUE, "Display Lasso");
    for (DinoPlayer& player : players)
        player.DrawLasso();
    XDino_ProfileEnd();

    XDino_ProfileBegin(DinoColor_YELLOW, "Display Entities");
    std::sort(entities.begin(), entities.end(), CompareEntity);
    for (DinoEntity* entity : entities)
        entity->Display(timeSinceStart);
    XDino_ProfileEnd();
    
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