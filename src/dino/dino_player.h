#pragma once

#include <dino/xdino.h>

class DinoPlayer {

    DinoVec2 pos = {};
    bool goingRight = false;
    bool idle;
    bool walking;
    bool running;
    int playerIndex = 0;

public:
    void Init(int idx, DinoVec2 posInit);
    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);
};