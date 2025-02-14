#pragma once
#include "Tile.h"
#include "xdino.h"

class TileMap
{
public:
    explicit TileMap();
    explicit TileMap(int scale, int width, int height);
    
    void Init();
    void Update(float deltaTime) const;   
    void Draw() const;

private:
    DinoVec2 _top_pos;
    int _width, _height;
    int _scale;

    std::vector<Tile*> _tiles;
    std::vector<Tile*> _props;
};
