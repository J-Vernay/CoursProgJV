#include "dino_entities.h"
#include "dino/xdino.h"

bool DinoEntity::isAbove(DinoEntity& other)
{
    return playerPos.y < other.playerPos.y;
}