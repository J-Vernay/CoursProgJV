#pragma once

#include "dino_player.h"
#include  "dino/xdino.h"
class  RopeManager
{
public:
    void Update();

    static std::vector<std::vector<DinoVec2>> allRopes;

    void ResolveCollision(DinoObject::DinoLasso& lassoA, DinoObject::DinoLasso& lassoB);
};