#include <dino/dino_entity.h>

DinoVec2 DinoEntity::GetPos()
{
    return pos;
}

void DinoEntity::SetPos(DinoVec2 newPos)
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

void DinoEntity::OnTerrainBorder()
{
    // Ne fait rien par défaut.
}

void DinoEntity::Update(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize)
{
};

void DinoEntity::Draw(double timeSinceStart)
{
};