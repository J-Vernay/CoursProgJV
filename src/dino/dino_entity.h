#pragma once
#include <dino/xdino.h>

class DinoEntity {
protected:
    DinoVec2 m_pos = {};

public:
    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);
};