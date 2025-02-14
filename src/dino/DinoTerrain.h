#pragma once

#include <dino/xdino.h>

class DinoTerrain {

public:
    DinoTerrain();

    void DrawWater(DinoVec2 renderSize);

    void DrawLand(DinoVec2 renderSize, DinoVec2 landSize);

};