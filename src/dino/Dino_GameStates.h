#pragma once

#include "game_state.h"
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_score.h>
#include <dino/xdino.h>
#include <dino/dino_lasso.h>
#include "dino_tree.h"
#include <memory>
#include <vector>


class DinoGameState {
public:
    static constexpr DinoVec2 RENDER_SIZE = {480, 360};

    static constexpr DinoColor PLAYER_COLORS[4] = {
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

    uint64_t m_vbuffID_nom = 0;
    DinoVec2 m_textSize_nom = {};
    double m_lastTime = 0.0;
    bool gamePreviouslyEnded = false;

    DinoAnimalSpawner g_spawner;
    DinoScoreManager g_scoreManager;

    std::vector<PlayerState> g_players;
    std::vector<int> freePlayerIndices = {0, 1, 2, 3};
    std::vector<DinoGamepadIdx> unassignedGamepads;
    std::vector<DinoGamepadIdx> assignedGamepads;


    void Init(double timeSinceStart);
    void Update(float deltaTime, double timeSinceStart);
    void DrawState(float deltaTime, double timeSinceStart);
    void ChangeState(std::unique_ptr<GameState> newState, double timeSinceStart);
    void Shut();

private:
    std::unique_ptr<GameState> currentState;
    std::unique_ptr<GameState> nextState;


};