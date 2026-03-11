#pragma once

#include <dino/xdino.h>

struct DinoControllerFields {

    // Fields
private:
    DinoVec2 m_dinoPos;
    double m_dinoCurrentSpeed;
    bool m_dinoGoingLeft = false;

    float m_dinoAnimElapsed = 0;
    float m_dinoDamageAnimTimer;
    int m_currFrame = 0;
    int m_dinoColor = 0;

    bool m_dinoCanTakeDamage = true;

public:
    uint64_t vbufID_dino;

    // Methods
private:
    uint64_t GenDinoVertexBuffer(DinoGamepad gamepad, float deltaTime);

public:
    void Init(int playerCount);
    // void Shut();

    void DinoMovement(DinoGamepad gamepad, float deltaTime);
    void DrawDino(DinoGamepad gamepad, float deltaTime, uint64_t texID_dino);
};