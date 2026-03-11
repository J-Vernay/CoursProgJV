/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "Terrain.h"
#include "DinoPlayer.h"
#include "../../build/Animal.h"
#include "../../build/CollisionManager.h"

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

// Variable globale pour l'affichage de debug.
int g_debugScroll = 1;

std::vector<DinoPlayer> players;
std::vector<Animal> animals;
std::vector<Agent*> agentsList;
float elapsedTimeBeforeNewAnimal = 0;

Terrain terrain;
DinoVec2 _MINI_terrainBounds = {};
DinoVec2 _MAXI_terrainBounds = {};

CollisionManager collisionManager = CollisionManager();

// Constantes.

void Dino_GameInit()
{
    DinoVec2 windowSize = {420, 360};
    XDino_SetRenderSize(windowSize);

    terrain.SetUpTerrain(windowSize);
    _MINI_terrainBounds = terrain._MINI_terrainBound;
    _MAXI_terrainBounds = terrain._MAXI_terrainBound;

    players.reserve(NUMBER_PLAYERS);
    animals.reserve(1000);
    agentsList.reserve(2000);
    
    for (int i = 0; i < NUMBER_PLAYERS; i++)
    {
        players.emplace_back(DINO_SPEED, i, _MINI_terrainBounds, _MAXI_terrainBounds, collisionManager);
        agentsList.push_back(&players.back());
    }

    collisionManager.SetPlayers(&players);
    collisionManager.SetAgents(agentsList);
    
    for (DinoPlayer& player : players)
    {
        player.Start();
    }
}

float currentFrameAnim = 0;

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    DinoVec2 windowSize = {420, 360};
    XDino_SetRenderSize(windowSize);

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    
    //Terrain
    terrain.Update(deltaTime);

    collisionManager.Update(deltaTime);
    
    elapsedTimeBeforeNewAnimal += deltaTime;
    
    if (elapsedTimeBeforeNewAnimal >= 1)
    {
        int animalType = XDino_RandomInt32(0, 7);
        elapsedTimeBeforeNewAnimal = 0;
        animals.emplace_back(animalType,_MINI_terrainBounds, _MAXI_terrainBounds, windowSize);
        agentsList.push_back(&animals.back());
        collisionManager.SetAgents(agentsList);
        collisionManager.SetAnimals(&animals);
    }
   
    collisionManager.ShuffleByVerticalPosition(agentsList);
    collisionManager.CheckLassoCollisionPlayer(players);
    
    for (Agent* agent : agentsList)
    {
        agent->Update(deltaTime);
    }
    
    collisionManager.ApplyPlayersCollision(agentsList);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }
    
    {
        std::string text = std::format("BOURRICAUD Antoine");
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "BOURRICAUD Antoine");
        DinoVec2 position = {0, windowSize.y - text.size()};
        XDino_Draw(vbufID, XDino_TEXID_FONT, position, 2);
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
    for (DinoPlayer& player : players) {
        player.ShutDown();
    }

    
}