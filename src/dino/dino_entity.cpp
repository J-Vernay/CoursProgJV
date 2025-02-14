#include <dino/dino_entity.h>

DinoVec2 DinoEntity::GetPos()
{
    return m_pos;
}

void DinoEntity::SetPos(DinoVec2 newPos)
{
    m_pos = newPos;
}

void DinoEntity::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    bool bTerrainCollide = false;
    if (m_pos.y < a.y) {
        m_pos.y = a.y;
        bTerrainCollide = true;
    }
    if (m_pos.y > b.y) {
        m_pos.y = b.y;
        bTerrainCollide = true;
    }
    if (m_pos.x < a.x) {
        m_pos.x = a.x;
        bTerrainCollide = true;
    }
    if (m_pos.x > b.x) {
        m_pos.x = b.x;
        bTerrainCollide = true;
    }
    if (bTerrainCollide) {
        OnTerrainBorder();
    }
}

void DinoEntity::OnTerrainBorder()
{
    // Ne fait rien par défaut.
}

void DinoEntity::Update(float deltaTime)
{
    // Ne fait rien par défaut.
}

void DinoEntity::Draw(double timeSinceStart)
{
    // Ne fait rien par défaut.
}