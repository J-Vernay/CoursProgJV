#pragma once
#include "xdino.h"

class Entity
{
public:
    explicit Entity();
    explicit Entity(const std::string& tex);
    explicit Entity(const std::string& tex, DinoVec2 pos, int radius);

    virtual ~Entity() = default;
    
    virtual void Update(float detlaTime);
    virtual void Draw();
    
protected:
    std::string tex;
    DinoVec2 pos;
    int radius;
};
