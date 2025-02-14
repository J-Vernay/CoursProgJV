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