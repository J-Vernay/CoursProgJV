#pragma once
#include "dino_entities.h"
#include "dino/xdino.h"

class DinoAnimals : public DinoEntity {
public :
    void UpdatePlayer(float time);
    void DrawPlayer(double time);
    void Init();
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