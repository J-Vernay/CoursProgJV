#pragma once

#include "dino_entity.h"

#include <dino/xdino.h>

class DinoPlayer : public DinoEntity {
    bool m_bLeft = false;
    double m_endHitAnim = 0;
    bool m_bMoving = false;
    bool m_bPressedRun = false;
    int m_idxPlayer = 0;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

    void ReactLimit() override;

public:
    std::vector<DinoVec2> m_pastPositions;
    void Init(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void Draw(double timeSinceStart) override;
    void Shut();

    static void InitStatic();
    static void ShutStatic();
};