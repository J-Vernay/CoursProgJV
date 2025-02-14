#include "dino_entity.h"
#include <dino/xdino.h>

void dino_entity::SetPos(DinoVec2 newPos)
{
    pos = newPos;
}

DinoVec2 dino_entity::GetPos()
{
    return pos;
}

void dino_entity::ApplyTerain(DinoVec2 a, DinoVec2 b)
{
    bool terrainCollide = false;

    if (pos.y < a.y) {
        pos.y = a.y;
        terrainCollide = true;
    }

    if (pos.y > b.y) {
        pos.y = b.y;
        terrainCollide = true;
    }

    if (pos.x < a.x) {
        pos.x = a.x;
        terrainCollide = true;
    }

    if (pos.x > b.x) {
        pos.x = b.x;
        terrainCollide = true;
    }

    if (terrainCollide) {
        OnTerrainBorder();
    }
}

void dino_entity::OnTerrainBorder()
{
    //Ne fait rien
}

void dino_entity::Update(float deltaTime)
{
    //Ne fait rien
}

void dino_entity::Draw(double timeSinceStart)
{
    //Ne fait rien
}