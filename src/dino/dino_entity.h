#pragma once
#include "xdino.h"

class DinoEntity {
    DinoVec2 m_pos = {};

public:
    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);
};