#pragma once
#include <dino/xdino.h>
#include <dino/dino_terrain.h>

class DinoEntity {
protected:
    DinoVec2 m_pos = {};

    // Appelée quand ApplyLimit() détecte le bord du terrain
    virtual void ReactLimit(bool xChanged) = 0;

public:
    virtual void Draw(double timeSinceStart) = 0;
    void ApplyTerrainLimit(DinoTerrain terrain);
    virtual void ReactLoop(double timeSinceStart) = 0;

    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);
    DinoVec2 GetPos();

    static bool CompareVerticalPos(DinoEntity* a, DinoEntity* b);
};