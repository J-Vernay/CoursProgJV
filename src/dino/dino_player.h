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
    std::vector<DinoVec2> m_lestlasso;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

    void ReactLimit() override;

public:
    std::vector<DinoVec2> m_pastPositions;
    void Init(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void CheckPlayerIntersec(DinoPlayer& player1, DinoPlayer& player2);
    void Draw(double timeSinceStart) override;
    void DrawLasso();
    void Shut();

    static void InitStatic();
    static void ShutStatic();
};