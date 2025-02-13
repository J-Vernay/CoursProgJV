#pragma once
#include "xdino.h"

#include <string>

class Tile
{
public:
    explicit Tile();
    explicit Tile(std::string tex);
    explicit Tile(std::string tex, DinoVec2 pos);

    void Update(float deltaTime);
    void Draw();

private:
    std::string tex;
    DinoVec2 pos;

    uint16_t i;
    uint16_t j;
};
