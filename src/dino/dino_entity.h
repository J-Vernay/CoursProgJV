#pragma once
#include <dino/xdino.h>

class DinoEntity {
protected:
    DinoVec2 m_pos = {};
    virtual void ReactLimit() = 0;

    // Appelée quand ApplyLimit() détecte le bord du terrain
    //virtual void ReactLimit() = 0;

public:
    virtual void Draw(double timeSinceStart) = 0;
    virtual void ReactLoop(double timeSinceStart) = 0;

    void ApplyLimit(DinoVec2 min, DinoVec2 max);
    DinoVec2 GetPos();

    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);

    static bool CompareVerticalPos(DinoEntity* a, DinoEntity* b);
};