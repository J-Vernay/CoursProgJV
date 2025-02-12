#pragma once
#include "xdino.h"

class DinoPlayer {
private:
    DinoGamepadIdx gamepadIdx;
    DinoVec2 pos = {};
    bool bMirror = false;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;

public:
    DinoPlayer();
    ~DinoPlayer();

    void Init(DinoGamepadIdx gamepadIdx, DinoVec2 pos);

    void Update(float deltaTime);
    void Draw(double timeSinceStart);
};