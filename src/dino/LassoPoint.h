// LassoPoint.h
#pragma once
#include <dino/xdino.h>

struct LassoPoint {
    DinoVec2 position;
    uint64_t buff = 0;
    float timeAlive = 0.0f;
};