#pragma once
#include "dino/xdino.h"

class DinoPlayer {
public :
    void UpdatePlayer(float time);
    void DrawPlayer(float time);
    void Init(DinoVec2 pos);
    DinoVec2 playerPos = {};

private :
    bool g_bMiror = false;
    bool isIdle;
    float baseSpeed = 1;
    bool isWalking;
    bool isRunning;
};