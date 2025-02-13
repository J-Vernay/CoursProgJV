#pragma once

#include <dino/xdino.h>

class DinoPlayer {
    DinoVec2 playerPos = {};
    bool isMirror = false;
    bool isIdle;
    bool isWalking;
    bool isRunning;
    int indexPlayer = 0;

public:
    void Init(int index, DinoVec2 posInit);
    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);
};