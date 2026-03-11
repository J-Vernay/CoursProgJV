#include <dino/dino_entity.h>

void DinoEntity::ResolveCollision(DinoEntity& entityA, DinoEntity& entityB)
{
    DinoVec2& a = entityA.m_pos;
    DinoVec2& b = entityB.m_pos;
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