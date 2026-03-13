/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/Terrain.h>
#include <dino/DinoPlayer.h>
#include <dino/GameManager.h>

#include <dino/Animal.h>
#include <dino/CollisionManager.h>
#include <dino/DinoArray.h>

#include <__msvc_ostream.hpp>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>
#include <iostream>
#include <cmath>

// Variables globales.

class DinoPlayer;
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
uint64_t texID_imageMilieu;

constexpr float DINO_SPEED = 150.f;
constexpr int NUMBER_PLAYERS = 2;

constexpr float WORLD_OFFSET_X = 420.f / 4.f;
constexpr float WORLD_OFFSET_Y = 360.f / 4.f;

int g_debugScroll = 1;

DinoArray<DinoPlayer*>* players = nullptr;
DinoArray<Animal*>* animals = nullptr;
DinoArray<Agent*>* agentsList = nullptr;

float elapsedTimeBeforeNewAnimal = 0;
float maxTimeBeforeNewAnimal = 0.5f;
float timeToSpawnAnimal = 2;

Terrain* terrain = nullptr;

DinoVec2 _MINI_terrainBounds = {};
DinoVec2 _MAXI_terrainBounds = {};

CollisionManager* collisionManager;

GameManager* gameManager;

// Constantes.

void Dino_GameInit()
{
    DinoVec2 windowSize = {420, 360};
    XDino_SetRenderSize(windowSize);

    gameManager = new GameManager();
    
    terrain = new Terrain();
    
    terrain->SetUpTerrain();
    _MINI_terrainBounds = terrain->_MINI_terrainBound;
    _MAXI_terrainBounds = terrain->_MAXI_terrainBound;
    
    players = new DinoArray<DinoPlayer*>(0, NUMBER_PLAYERS);
    animals = new DinoArray<Animal*>(0, 1000);
    agentsList = new DinoArray<Agent*>(0, 1000);

    collisionManager = new CollisionManager(*gameManager);
    
    for (int i = 0; i < NUMBER_PLAYERS; i++)
    {
        DinoPlayer* player = new DinoPlayer(DINO_SPEED, i, _MINI_terrainBounds, _MAXI_terrainBounds, *collisionManager, *gameManager);
        players->AddBack(player);
        agentsList->AddBack(player);
    }

    collisionManager->SetPlayers(players);
    collisionManager->SetAgents(agentsList);
    
    for (DinoPlayer* player : *players)
    {
        player->Start();
    }

    gameManager->SetPlayers(players);
    
    gameManager->StartGame(4);
}

float currentFrameAnim = 0;

void Dino_GameFrame(double timeSinceStart)
{
    DinoVec2 windowSize = {420, 360};
    XDino_SetRenderSize(windowSize);

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    
    terrain->Update(deltaTime);
    
    elapsedTimeBeforeNewAnimal += deltaTime;

    timeToSpawnAnimal = 0.5f + (2- 0.5f) * gameManager->GetCurrentT_Time();
    
    if (elapsedTimeBeforeNewAnimal >= timeToSpawnAnimal && !gameManager->IsPaused())
    {
        int animalType = XDino_RandomInt32(0, 7);
        elapsedTimeBeforeNewAnimal = 0;

        Animal* animal = new Animal(animalType,_MINI_terrainBounds, _MAXI_terrainBounds, *gameManager);
        
        animals->AddBack(animal);
        agentsList->AddBack(animal);
        
        collisionManager->SetAgents(agentsList);
        collisionManager->SetAnimals(animals);
    }

    for (DinoPlayer* player : *players) {
        player->UpdateLasso(deltaTime);
    }
    
    for (Agent* agent : *agentsList)
    {
        agent->Update(deltaTime);
    }

    gameManager->UpdateGame(deltaTime);    

    //Collision Manager
    collisionManager->Update(deltaTime);
    collisionManager->ShuffleByVerticalPosition(agentsList);
    collisionManager->CheckLassoCollisionPlayer(players);
    collisionManager->ApplyPlayersCollision(*agentsList);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 1);
        XDino_DestroyVertexBuffer(vbufID);
    }
    
    {
        std::string text = std::format("BOURRICAUD Antoine");
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "BOURRICAUD Antoine");
        DinoVec2 position = {windowSize.x - vs.size(), 0};
        XDino_Draw(vbufID, XDino_TEXID_FONT, position, 1);
        XDino_DestroyVertexBuffer(vbufID);
    }

#if !XDINO_RELEASE
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.shoulder_left) {
        int diff = 0;
        if (keyboard.dpad_up)
            diff -= 1;
        if (keyboard.dpad_down)
            diff += 1;
        XDino_DrawStats(diff);
    }
#endif
}

void Dino_GameShut()
{
    terrain->ShutDown();
    delete terrain;
    terrain = nullptr;
    
    for (DinoPlayer* player : *players)
    {
        player->ShutDown();
        delete player;
    }
    players->Clear();
    delete players;
    players = nullptr;

    for (Animal* animal : *animals) {
        delete animal;
    }
    
    animals->Clear();
    delete animals;
    animals = nullptr;

    delete agentsList;
    agentsList = nullptr;


    gameManager->ShutDown();
    delete gameManager;
    gameManager = nullptr;

    delete collisionManager;
    collisionManager = nullptr;
}