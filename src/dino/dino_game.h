#pragma once

#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_lasso.h>

enum class EGameState { Lobby, InGame, Paused };

struct PlayerState {
    DinoGamepadIdx gamepadIdx;
    DinoGamepad gamepad;
    DinoPlayer dino;
    DinoLasso lasso;

    PlayerState(DinoGamepadIdx idx, DinoGamepad gp, int idxPlayer, DinoColor color)
        : gamepadIdx(idx)
          , gamepad(gp)
          , dino(idxPlayer)
          , lasso(color)
    {
    }
};