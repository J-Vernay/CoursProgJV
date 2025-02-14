#pragma once
#include "dino/xdino.h"

class DinoEntity {
public :
    bool isAbove(DinoEntity& other);
    void ApplyTerrain(DinoVec2& a, DinoVec2& b);
    DinoVec2 GetPos();
    void Set(DinoVec2& newPos);
    DinoVec2 playerPos = {};
};