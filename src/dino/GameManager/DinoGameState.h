#pragma once

#include <dino/LassoManager.h>
#include <dino/Entities/dino_EntityManager.h>

#include <deque>
#include <memory>
#include <optional>
#include <unordered_map>
#include <dino/GameManager/GameState.h>
#include <dino/GameManager/dino_terrain.h>
#include <dino/Entities/dino_player.h>
#include <dino/Entities/dino_animal.h>

class DinoGameState {

public:
    
    uint64_t texID_dino;
    
    DinoVec2 g_terrainTopLeft = {(480 - 256) / 2, (360 - 192) / 2};
    
    dino_EntityManager g_dinoEntityManager;
    LassoManager g_lassoManager;
    std::unordered_map<DinoGamepadIdx, dino_player> gamepadDino_map;
    std::unordered_map<DinoGamepadIdx, DinoGamepad> lastFrameInputs_map;
    std::deque<dino_animal> animals;

    void Init();
    void Update(float deltaTime, double timeSinceStart);
    void ChangeGameState(std::unique_ptr<GameState> newGameState);
    void Shutdown();

private:
    std::unique_ptr<GameState> currentGameState;
    std::unique_ptr<GameState> nextState;
};