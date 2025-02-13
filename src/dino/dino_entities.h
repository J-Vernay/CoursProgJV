#pragma once
#include "dino/xdino.h"

class DinoEntity {
public :
    bool isAbove(DinoEntity& other);
    DinoVec2 playerPos = {};
};