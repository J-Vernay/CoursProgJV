#pragma once

#include "game_state.h"
#include <dino/Dino_GameStates.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_score.h>
#include <dino/xdino.h>
#include <dino/dino_lasso.h>
#include "dino_tree.h"
#include <optional>
#include <vector>

class LobbyState : public GameState {

public:
    LobbyState(DinoGameState* dino_gameState, int season);

    void EnterState(double timeSinceStart) override;
    void UpdateState(float deltaTime, double timeSinceStart) override;
    void DrawState(float deltaTime, double timeSinceStart) override;
    void ExitState() override;

private:
    DinoTerrain m_terrain;
    std::vector<DinoTree> m_trees;
    std::vector<DinoEntity*> m_entities;
    int m_season;


};