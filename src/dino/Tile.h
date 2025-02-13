#pragma once
#include "Entity.h"
#include "xdino.h"

#include <string>

class Tile : public Entity
{
public:
    explicit Tile();
    explicit Tile(const std::string& tex_d);
    explicit Tile(const std::string& tex_d, DinoVec2 pos_d);
    explicit Tile(const std::string& tex_d, DinoVec2 pos_d, uint16_t u, uint16_t v);
    
    void Update(float deltaTime) const;
    void Draw() const;

private:
    uint16_t i;
    uint16_t j;
};
