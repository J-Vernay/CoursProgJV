#pragma once
#include "dino/xdino.h"

class Agent
{
public:
    DinoVec2 position = {};
    int ID = -1;
    bool readyToDelete = false;
    
    virtual void SetPosition(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    virtual ~Agent() = default;
    
    virtual void Update(float deltaTime) {}

    virtual void TakeDamage() {}
};
