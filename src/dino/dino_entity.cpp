#include "dino_terrain.h"

#include <algorithm>
#include <dino/dino_entity.h>

void DinoEntity::ResolveCollision(DinoEntity& playerA, DinoEntity& playerB)
{
    DinoVec2& a = playerA.m_pos;
    DinoVec2& b = playerB.m_pos;
    float ab = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));

    if (ab == 0 || ab >= 16)
        return;

    float dx = (16 - ab) / (2 * ab) * (b.x - a.x);
    float dy = (16 - ab) / (2 * ab) * (b.y - a.y);
    a.x -= dx;
    a.y -= dy;
    b.x += dx;
    b.y += dy;
}

DinoVec2 DinoEntity::GetPos()
{
    return m_pos;
}

void DinoEntity::ApplyTerrainLimit(DinoTerrain terrain)
{
    DinoVec2 pos = m_pos;
    DinoVec2 topLeftCorner = terrain.GetTopLeft();
    DinoVec2 bottomRightCorner = terrain.GetBottomRight();
    pos.x = std::clamp(pos.x, topLeftCorner.x, bottomRightCorner.x);
    pos.y = std::clamp(pos.y, topLeftCorner.y, bottomRightCorner.y);

    if (pos.x != m_pos.x) {
        // Pour DinoPlayer: ne rien faire
        // Pour DinoAnimal: changer la direction
        ReactLimit(true);
    }
    else if (pos.y != m_pos.y) {
        ReactLimit(false);
    }
    m_pos = pos;
}

bool DinoEntity::CompareVerticalPos(DinoEntity* a, DinoEntity* b)
{
    return a->m_pos.y < b->m_pos.y;
}