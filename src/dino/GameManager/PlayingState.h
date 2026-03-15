#pragma once

#include <optional>
#include <unordered_map>
#include <format>
#include <dino/dino_geometry.h>
#include <dino/GameManager/dino_terrain.h>
#include <dino/GameManager/GameState.h>
#include "DinoGameState.h"


class PlayingState : public GameState {

public:
    PlayingState(DinoGameState* dino_game_state, int season);
    
    void EnterState() override;
    void UpdateState(float deltaTime, double timeSinceStart) override;
    void ExitState() override;

private:
    float g_spawnTimer = 0;
    const float g_baseSpawnDelay = 3;
    const float g_endSpawnDelay = 0.5f;

    const float PLAYING_TIME = 60;
    float g_timeLeft;
    
    bool isPaused = false;
    
    std::optional<dino_terrain> m_dinoTerrain;
    std::unordered_map<DinoGamepadIdx, DinoGamepad> lastFrameInputs_map;
};