#pragma once
#include <dino/xdino.h>

class DinoEntity {
protected:
    DinoVec2 m_pos = {};

public:
    virtual void Draw(double timeSinceStart) = 0;
    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);
    static bool CompareVerticalPos(DinoEntity* a, DinoEntity* b);
    DinoVec2 GetPos();
    virtual void ReactLoop(double timeSinceStart) = 0;
};