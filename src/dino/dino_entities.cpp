#include "dino_entities.h"
#include "dino/xdino.h"

bool DinoEntity::isAbove(DinoEntity& other)
{
    return playerPos.y < other.playerPos.y;
}

void DinoEntity::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    if (playerPos.y < a.y)
        playerPos.y = a.y;
    if (playerPos.y > b.y)
        playerPos.y = b.y;
    if (playerPos.x < a.x)
        playerPos.x = a.x;
    if (playerPos.x > b.x)
        playerPos.x = b.x;
}