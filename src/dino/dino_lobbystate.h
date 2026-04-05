#pragma once

#include "dino_gamestate.h"
#include <dino/dino_tree.h>
#include <vector>

class LobbyState : public GameState {
public:
    explicit LobbyState(double timeSinceStart, const GameContext& ctx);

    GameState* Update(double timeSinceStart, float deltaTime, GameContext& ctx) override;
    void Draw(double timeSinceStart, const GameContext& ctx) const override;
private:
    void SpawnTrees(double timeSinceStart, const GameContext& ctx);
    void HandlePlayerJoining(GameContext& ctx);

    std::vector<DinoTree> m_trees;

    static constexpr DinoColor PLAYER_COLORS[4] = {
        DinoColor_BLUE,
        DinoColor_RED,
        DinoColor_YELLOW,
        DinoColor_GREEN,
    };
};