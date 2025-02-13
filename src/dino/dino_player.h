#pragma once
#include "xdino.h"

class DinoPlayer {
private:
    DinoGamepadIdx gamepadIdx;
    DinoVec2 pos;
    bool bMirror;

    bool bIdle;
    bool bWalking;
    bool bRunning;

public:
    DinoPlayer();
    ~DinoPlayer();

    void Init(DinoGamepadIdx gamepadIdx, DinoVec2 pos);

    void Update(float deltaTime);
    void Draw(double timeSinceStart);

    static bool DinoPlayerCompare(const DinoPlayer& a, const DinoPlayer& b);
};