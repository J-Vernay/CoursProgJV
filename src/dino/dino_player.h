#pragma once

#include "dino_entity.h"

#include <dino/xdino.h>

class DinoPlayer : public DinoEntity {
private:
    bool m_bLeft = false;
    double m_endHitAnim = 0;
    bool m_bMoving = false;
    bool m_bPressedRun = false;
    int m_idxPlayer = 0;

    static uint64_t s_texID;

    DinoVertexBuffer GenerateVertexBuffer(double timeSinceStart);

    void ReactLimit() override;
    void ReactLoop(double timeSinceStart) override;

public:
    DinoPlayer(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void Draw(double timeSinceStart) override;
    void Shut();

    static void InitStatic();
    static void ShutStatic();
    
    std::vector<DinoVec2> listPosition = {};
};