#pragma once
#include "dino\xdino.h"

class SpriteRenderer
{
public:
    virtual DinoVec2 GetPosition() = 0;
    virtual void SetPosition(DinoVec2 newPosition) = 0;

    virtual void Draw() = 0;
    
};
