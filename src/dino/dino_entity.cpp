#include <dino/dino_entity.h>

#include <cmath>

DinoVec2 DinoEntity::GetPos() const
{
    return m_pos;
}

void DinoEntity::ClampToTerrain(DinoTerrain const& terrain)
{
    m_pos = terrain.ClampPos(m_pos);
}

void DinoEntity::ResolveCollision(DinoEntity& entity1, DinoEntity& entity2)
{
    if (&entity1 == &entity2)
        return;
    DinoVec2 pos1 = entity1.m_pos;
    DinoVec2 pos2 = entity2.m_pos;
    DinoVec2 vec = {pos2.x - pos1.x, pos2.y - pos1.y};
    float dist = std::hypot(vec.x, vec.y);
    if (0 < dist && dist < 16) {
        float f = (16 - dist) / (2 * dist);
        entity1.m_pos.x -= f * vec.x;
        entity1.m_pos.y -= f * vec.y;
        entity2.m_pos.x += f * vec.x;
        entity2.m_pos.y += f * vec.y;
    }
}