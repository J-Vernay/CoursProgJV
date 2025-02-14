#include "dino_entity.h"

void DinoEntity::Update(float deltaTime)
{
    // Do nothing by default.
}

void DinoEntity::Display(double timeSinceStart)
{
    // Do nothing by default.
}

void DinoEntity::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    bool bTerrainCollide = false;
    if (pos_.y < a.y)
    {
        pos_.y = a.y;
        bTerrainCollide = true;
    }
    if (pos_.y > b.y)
    {
        pos_.y = b.y;
        bTerrainCollide = true;
    }
    if (pos_.x < a.x)
    {
        pos_.x = a.x;
        bTerrainCollide = true;
    }
    if (pos_.x > b.x)
    {
        pos_.x = b.x;
        bTerrainCollide = true;
    }
    if (bTerrainCollide)
        OnTerrainBorder();
}

void DinoEntity::OnTerrainBorder()
{
    // Do nothing by default.
}


