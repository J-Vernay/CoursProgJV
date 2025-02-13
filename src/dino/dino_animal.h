#pragma once

#include <dino//dino_draw_utils.h>

class dino_animal {
    DinoVec2 pos = {};
    DinoVec2 direction = {};
    int32_t idxAnimal;

    bool isMirror = false;
    bool isIdle = false;
    bool isWalking = false;

public:
    void Init(DinoVec2 initPos, int32_t indexAnimal);
    void UpdateAnimal(float deltaTime);
    void DrawAnimal(double timeSinceStart);
    bool IsAbove(dino_animal& other);
};