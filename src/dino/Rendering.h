#pragma once

#include "Component/SpriteRenderer.h"

#include <vector>

#include "dino/xdino.h"

class Rendering
{
    public:
    void Update(float deltaTime);
    
    static std::vector<SpriteRenderer*> spriteRenderer;
};
