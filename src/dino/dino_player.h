#pragma once

#include <dino/xdino.h>

// Hello Corentin

class DinoPlayer {
    DinoVec2 m_pos = {};
    bool m_bLeft = false;
    double m_endHitAnim = 0;
    bool m_bMoving = false;
    bool m_bPressedRun = false;
    int m_idxPlayer = 0;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void Draw(double timeSinceStart);
    void Shut();

    static void InitStatic();
    static void ShutStatic();
};