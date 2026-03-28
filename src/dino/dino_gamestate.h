#pragma once

#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_lasso.h>
#include <vector>

struct PlayerState {
    DinoGamepadIdx gamepadIdx;
    DinoGamepad    gamepad;
    DinoColor      playerColor;
    DinoPlayer     dino;
    DinoLasso      lasso;
    int            score = 0;
};

struct GameContext {
    std::vector<DinoGamepadIdx>& unassignedGamepads;
    std::vector<PlayerState>&    players;
    DinoTerrain&                 terrain;
    DinoVec2                     renderSize;
};

class GameState {
public:
    virtual ~GameState() = default;
    virtual GameState* Update(double timeSinceStart, float deltaTime, GameContext& ctx) = 0;
    virtual void       Draw(double timeSinceStart, const GameContext& ctx) const = 0;
};