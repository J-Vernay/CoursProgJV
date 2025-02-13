#pragma once
#include "dino/xdino.h"

class DinoPlayer {
public :
    void UpdatePlayer(float time);
    void DrawPlayer(double time);
    void Init(DinoVec2 pos, DinoGamepadIdx idx, int indexGraph);
    bool isAbove(DinoPlayer& other);
    DinoVec2 playerPos = {};

private :
    DinoGamepadIdx gamepadIdx;
    bool g_bMiror = false;
    int indexG = 0;
    bool isIdle;
    float baseSpeed = 300;
    bool isWalking;
    bool isRunning;
};