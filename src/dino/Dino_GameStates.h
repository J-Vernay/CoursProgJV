#pragma once

#include "game_state.h"
#include <dino/Lobby_state.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_score.h>
#include <dino/xdino.h>
#include <dino/dino_lasso.h>
#include "dino_tree.h"
#include <memory>
#include <vector>

constexpr DinoVec2 RENDER_SIZE = {480, 360};

constexpr DinoColor PLAYER_COLORS[4] = {
    DinoColor_BLUE,
    DinoColor_RED,
    DinoColor_YELLOW,
    DinoColor_GREEN,
};

struct PlayerState {
    DinoGamepadIdx gamepadIdx;
    DinoGamepad gamepad;
    DinoPlayer player;
    DinoLasso lasso;
};

class DinoGameState {
public:
    void Init();
    void Shut();
    void Frame(double timeSinceStart);

    void TransitionTo(std::unique_ptr<GameState> newState, double timeSinceStart);

    DinoTerrain terrain;
    DinoAnimalSpawner spawner;
    DinoScoreManager scoreManager;

    std::vector<PlayerState> players;
    std::vector<int> freePlayerIndices = {0, 1, 2, 3};
    std::vector<DinoGamepadIdx> unassignedGamepads;
    std::vector<DinoGamepadIdx> assignedGamepads;
    std::vector<DinoTree> trees;

    double chrono = 60.0;

private:
    std::unique_ptr<GameState> m_currentState;
    double m_lastTime = 0.0;

    uint64_t m_vbuffID_nom = 0;
    DinoVec2 m_textSize_nom = {};
};