#include "dino_entity.h"

DinoVec2 DinoEntity::GetPosition()
{
    return pos;
}

void DinoEntity::SetPosition(DinoVec2 newPos)
{
    pos = newPos;
}

void DinoEntity::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    bool bTerrainCollide = false;
    if (pos.y < a.y) {
        pos.y = a.y;
        bTerrainCollide = true;
    }
    if (pos.y > b.y) {
        pos.y = b.y;
        bTerrainCollide = true;
    }
    if (pos.x < a.x) {
        pos.x = a.x;
        bTerrainCollide = true;
    }
    if (pos.x > b.x) {
        pos.x = b.x;
        bTerrainCollide = true;
    }

    if (bTerrainCollide) {
        OnTerrainBorder();
    }
}

void DinoEntity::Update(float deltaTime)
{
    // Par défaut, ne fait rien.
}

void DinoEntity::Draw(double timeSinceStart)
{
    // Par défaut, ne fait rien.
}

void DinoEntity::OnTerrainBorder()
{
    // Par défaut, ne fait rien.
}