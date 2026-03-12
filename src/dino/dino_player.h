#pragma once

#include "dino_entity.h"

#include <dino/xdino.h>

class DinoPlayer : public DinoEntity {
private:
    std::vector<DinoVec2> m_lassoPoints;

    struct LassoPoint {
        DinoVec2 pos;
        double t;
    };

    std::vector<LassoPoint> m_lassoHistory;
    constexpr static float m_lassoMaxTime = 2.f;

    bool m_bLeft = false;
    double m_endHitAnim = 0;
    bool m_bMoving = false;
    bool m_bPressedRun = false;
    int m_idxPlayer = 0;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

    void ReactLimit() override;

public:
    void Init(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void UpdateLasso(double timeSinceStart, float deltaTime);
    void DrawLasso();
    void Draw(double timeSinceStart) override;
    void Shut();

    static void InitStatic();
    static void ShutStatic();
};