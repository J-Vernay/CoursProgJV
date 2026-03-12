#pragma once
#include <dino/xdino.h>

class DinoEntity {
protected:
    DinoVec2 m_pos = {};

    // Appelée quand ApplyLimit() détecte le bord du terrain
    virtual void ReactLimit() = 0;

public:
    void ApplyLimit(DinoVec2 min, DinoVec2 max);
    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);
};