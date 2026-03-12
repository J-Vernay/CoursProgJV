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

void DinoEntity::ApplyLimit(DinoVec2 min, DinoVec2 max)
{
    DinoVec2 pos = m_pos;
    if (pos.x < min.x)
        pos.x = min.x;
    if (pos.x > max.x)
        pos.x = max.x;
    if (pos.y < min.y)
        pos.y = min.y;
    if (pos.y > max.y)
        pos.y = max.y;

    if (pos.x != m_pos.x || pos.y != m_pos.y) {
        m_pos = pos;
        // Pour DinoPlayer: ne rien faire
        // Pour DinoAnimal: changer la direction
        ReactLimit();
    }
}

bool DinoEntity::CompareVerticalPos(DinoEntity* a, DinoEntity* b)
{
    return a->m_pos.y < b->m_pos.y;
}