#pragma once

#include "xdino.h"

class DinoAnimaux {
    DinoVec2 pos = {};
    bool m_bRight = false;
    bool m_bUp = false;
    bool m_bDown = false;
    double m_endHitAnim = 0;
    int m_idxAnimaux = 0;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(int idxAnimaux);
    void Update(double timeSinceStart, float deltaTime);
    void Draw(double timeSinceStart);
    void Shut();
};