#include <dino/dino_entity.h>

void DinoEntity::ResolveCollision(DinoEntity& entityA, DinoEntity& entityB)
{
    DinoVec2& a = entityA.m_pos;
    DinoVec2& b = entityB.m_pos;

    float ab = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    if (ab == 0 || ab >= 16)
        return;

    float dx = (16 - ab) / ab * (b.x - a.x);
    float dy = (16 - ab) / ab * (b.y - a.y);

    if (entityA.canBePushed && entityB.canBePushed) {
        // Both move — split the correction evenly
        a.x -= dx * 0.5f;
        a.y -= dy * 0.5f;
        b.x += dx * 0.5f;
        b.y += dy * 0.5f;
    }
    else if (entityA.canBePushed) {
        // Only A moves — absorb the full correction
        a.x -= dx;
        a.y -= dy;
    }
    else if (entityB.canBePushed) {
        // Only B moves — absorb the full correction
        b.x += dx;
        b.y += dy;
    }
    // Neither can be pushed — do nothing
}

bool DinoEntity::CompareVerticalPos(DinoEntity* a, DinoEntity* b)
{
    return a->m_pos.y < b->m_pos.y;
}

DinoVec2 DinoEntity::GetPos()
{
    return m_pos;
}