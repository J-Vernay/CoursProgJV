#pragma once
#include "Tile.h"

class AnimatedTile : public Tile
{
public:
    explicit AnimatedTile();
    explicit AnimatedTile(const std::string& tex_d, DinoVec2 pos_d, uint16_t u, uint16_t v);
    
    void Update(float deltaTime) override;

private:
    uint16_t offset;
    float animTimer;
    uint16_t k = 0;
};
