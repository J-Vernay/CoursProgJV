#pragma once

#include  "dino/xdino.h"
class RopeManager
{
public:
    void Update();

    static std::vector<std::vector<DinoVec2>> allRopes;


    void ScanForOtherPlayerIntersections();
};
