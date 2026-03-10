#pragma once

#include <dino/xdino.h>

struct DinoPlayer {
    DinoVec2 m_pos = {};
    bool m_bLeft = false;
    double m_endHitAnim = 0;

    uint64_t GenerateVertexBuffer(double timeSinceStart, bool bMoving, bool bPressedRun);
};