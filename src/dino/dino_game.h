#pragma once

#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_lasso.h>

enum class EGameState { Lobby, InGame, Paused };

constexpr double CHRONO_INIT = 60;

inline bool g_bGameOver = false;

struct PlayerState {
    DinoGamepadIdx gamepadIdx;
    DinoGamepad gamepad;
    DinoPlayer dino;
    DinoLasso lasso;
    int score = 0;
    int captureCountPerKind[8] = {0, 0, 0, 0};

    PlayerState(DinoGamepadIdx idx, DinoGamepad gp, int idxPlayer, DinoColor color)
        : gamepadIdx(idx)
          , gamepad(gp)
          , dino(idxPlayer)
          , lasso(color)
    {
    }
};