#pragma once

#include <dino//dino_draw_utils.h>
#include <dino/dino_entity.h>

class dino_animal : public dino_entity {
private:
    DinoVec2 direction = {};
    int32_t idxAnimal;

    bool isMirror = false;
    bool isIdle = false;
    bool isWalking = false;

    void OnTerrainBorder() override;

public:
    void Init(DinoVec2 initPos, int32_t indexAnimal);
    void UpdateAnimal(float deltaTime);
    void DrawAnimal(double timeSinceStart);
    bool IsAbove(dino_animal& other);
};