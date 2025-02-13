#pragma once
#include "xdino.h"

class Entity
{
public:
    explicit Entity();
    explicit Entity(const std::string& tex);
    explicit Entity(const std::string& tex, DinoVec2 pos);
    
protected:
    std::string tex;
    DinoVec2 pos;
};
