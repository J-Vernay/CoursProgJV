#pragma once

#include <dino/xdino.h>

struct DinoPlayer {
    DinoVec2 m_pos = {};
    bool m_bLeft = false;
    double m_endHitAnim = 0;
    uint64_t m_texID;
    bool m_bMoving = false;
    bool m_bPressedRun = false;

    void Init();
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void Shut();

    uint64_t GenerateVertexBuffer(double timeSinceStart);
};