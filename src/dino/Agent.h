#pragma once
#include "dino/xdino.h"

class Agent
{
public:
    enum AgentType
    {
        Pig1,
        Pig2,
        Cow1,
        Cow2,
        Sheep1,
        Sheep2,
        Autruche1,
        Autribute2,
        Player
    };
    
    DinoVec2 position = {};
    int ID = -1;
    bool readyToDelete = false;

    AgentType typeAgent;
    
    virtual void SetPosition(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    virtual ~Agent() = default;
    
    virtual void Update(float deltaTime) {}

    virtual void TakeDamage() {}
};
