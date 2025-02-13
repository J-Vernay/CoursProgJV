#pragma once
#include "dino/xdino.h"

struct DinoPlayer {
    DinoVec2 playerPos = {};
    bool g_bMiror = false;
    bool isIdle;
    bool isWalking;
    bool isRunning;
    void UpdatePlayer(float time);
    void DrawPlayer(float time);
};