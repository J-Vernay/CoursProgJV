#pragma once

#include <dino/xdino.h>

constexpr float CIRCLE_SPEED = 150.f;

class dino_player {
    DinoVec2 pos = {};
    bool bMirror = false;
    int idxPlayer = 0;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;

    DinoGamepadIdx m_idxGamepad;

public:
    void UpdatePlayer(float deltaTime);
    void DrawDino(double timeSinceStart);
    void InitDino(DinoVec2 position, int index, DinoGamepadIdx idxGamepad);
    bool IsAbove(dino_player& other);
};