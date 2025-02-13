#pragma once
#include "dino/dino_entities.h"
#include "dino/xdino.h"

class DinoPlayer : public DinoEntity {
public :
    void UpdatePlayer(float time);
    void DrawPlayer(double time);
    void Init(DinoVec2 pos, DinoGamepadIdx idx, int indexGraph);

private :
    DinoGamepadIdx gamepadIdx;
    bool g_bMiror = false;
    int indexG = 0;
    bool isIdle;
    float baseSpeed = 300;
    bool isWalking;
    bool isRunning;
};