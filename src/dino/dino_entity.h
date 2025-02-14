#pragma once
#include <dino/xdino.h>

class dino_entity {
protected:
    DinoVec2 pos;

public:
    DinoVec2 GetPos();
    void SetPos(DinoVec2 newPos);
};