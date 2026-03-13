#pragma once

#include "dino/xdino.h"



class  Collider
{
public:
    bool collider_enabled = true;
    int collider_radius;
    
    virtual DinoVec2 GetPosition() = 0;
    virtual void SetPosition(DinoVec2 newPosition) = 0;

    virtual void ReactLoop(float) = 0;

    
    
};
