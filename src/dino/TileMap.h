#pragma once
#include "xdino.h"

#include "Tile.h"

class TileMap
{
public:
    explicit TileMap();
    explicit TileMap(int scale, int width, int height);
    
    void Init();
    void Update(float deltaTime);   
    void Draw();

private:
    DinoVec2 topPos;
    int width, height;
    int scale;
    
    std::vector<Tile> tiles;
};
