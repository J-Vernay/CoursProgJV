#include "dino_entities.h"

#include "dino_animals.h"
#include "dino/xdino.h"

bool DinoEntity::isAbove(DinoEntity& other)
{
    return playerPos.y < other.playerPos.y;
}

void DinoEntity::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    bool bTerrainCollide = false;
    if (playerPos.y < a.y) {
        playerPos.y = a.y;
        bTerrainCollide = true;
    }
    if (playerPos.y > b.y) {
        playerPos.y = b.y;
        bTerrainCollide = true;
    }
    if (playerPos.x < a.x) {
        playerPos.x = a.x;
        bTerrainCollide = true;
    }
    if (playerPos.x > b.x) {
        playerPos.x = b.x;
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

DinoVec2 DinoEntity::GetPos()
{
    return playerPos;
}

void DinoEntity::Set(DinoVec2& newPos)
{
    playerPos = newPos;
}