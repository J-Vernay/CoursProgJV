#pragma once
#include "dino_draw_utils.h"

#include <dino/xdino.h>
#include <dino/dino_terrain.h>

class DinoEntity {
protected:
    DinoVec2 m_pos = {};

    bool m_isInGame = false;

    // Appelée quand ApplyLimit() détecte le bord du terrain
    virtual void ReactLimit(bool xChanged) = 0;

public:
    virtual void Draw(double timeSinceStart) = 0;
    void ApplyTerrainLimit(DinoTerrain terrain);
    virtual void ReactLoop(double timeSinceStart, int score, DinoColor lassoColor) = 0;

    virtual EAnimalKind GetKind();

    static void ResolveCollision(DinoEntity& entityA, DinoEntity& entityB);
    DinoVec2 GetPos();

    static bool CompareVerticalPos(DinoEntity* a, DinoEntity* b);
};