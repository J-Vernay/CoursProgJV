#pragma once

#include <deque>
#include <memory>
#include <optional>
#include <unordered_map>
#include <dino/LassoManager.h>
#include <dino/dino_ScoreManager.h>
#include <dino/Entities/dino_player.h>
#include <dino/Entities/dino_animal.h>
#include <dino/GameManager/GameState.h>
#include <dino/GameManager/LobbyState.h>
#include <dino/GameManager/dino_terrain.h>
#include <dino/Entities/dino_EntityManager.h>

class DinoGameState {

public:
    uint64_t texID_dino;

    DinoVec2 g_terrainTopLeft = {(480 - 256) / 2, (360 - 192) / 2};

    dino_EntityManager g_dinoEntityManager;
    LassoManager g_lassoManager;
    dino_ScoreManager g_scoreManager;
    std::unordered_map<DinoGamepadIdx, dino_player> gamepadDino_map;
    std::deque<dino_animal> animals;

    void Init(double timeSinceStart);
    void Update(float deltaTime, double timeSinceStart);
    void ChangeGameState(std::unique_ptr<GameState> newGameState);
    void Shutdown();

private:
    std::unique_ptr<GameState> currentGameState;
    std::unique_ptr<GameState> nextState;
};