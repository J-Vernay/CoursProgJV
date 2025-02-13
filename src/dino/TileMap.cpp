#include "TileMap.h"

TileMap::TileMap()
{
    width = 1;
    height = 1;
    scale = 1;
    topPos = {0,0};
}

TileMap::TileMap(const int scale, const int width, const int height)
{
    this->width = width;
    this->height = height;
    this->scale = scale;
    const float xo = 224;
    const float yo = 192;
    topPos = {xo, yo};
}

void TileMap::Update(float deltaTime)
{
    for (Tile& tile : this->tiles)
    {
        tile.Update(deltaTime);
    }
}

void TileMap::Draw()
{
    for (Tile& tile : this->tiles)
    {
        tile.Draw();
    }
}

void TileMap::Init()
{
    const auto s = width * height;
    tiles.resize(s);

    tiles[0] = Tile("terrain.png", topPos, 0, 16);
    const auto tr = DinoVec2 {topPos.x + 16 * scale * (width - 1), topPos.y};
    tiles[1] = Tile("terrain.png", tr, 32, 16);
    const auto bl = DinoVec2 {topPos.x, topPos.y + 16 * scale * (height - 1)};
    tiles[2] = Tile("terrain.png", bl, 0, 48);
    const auto br = DinoVec2 {topPos.x + 16 * scale * (width - 1), topPos.y + 16 * scale * (height - 1)};
    tiles[3] = Tile("terrain.png", br, 32, 48);

    int k = 4;
    for (int i = 1; i < width - 1; i++)
    {
        const auto cTopPos = DinoVec2 {topPos.x + 16 * scale * i, topPos.y}; 
        tiles[k] = Tile("terrain.png", cTopPos, 16, 16);
        const auto cBotPos = DinoVec2 {topPos.x + 16 * scale * i, topPos.y + 16 * scale * (height - 1)};
        tiles[k + 1] = Tile("terrain.png", cBotPos, 16, 48);

        k += 2;
    }
    
    for (int i = 1; i < height - 1; i++)
    {
        const auto cRightPos = DinoVec2 {topPos.x, topPos.y + 16 * scale * i};
        tiles[k] = Tile("terrain.png", cRightPos, 0, 32);
        const auto cLeftPos = DinoVec2 {topPos.x + 16 * scale * (width - 1), topPos.y + 16 * scale * i};
        tiles[k + 1] = Tile("terrain.png", cLeftPos, 32, 32);

        k += 2;
    }
    
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            const auto cCenterPos = DinoVec2 {topPos.x + 16 * scale * i, topPos.y + 16 * scale * j};
            tiles[k] = Tile("terrain.png", cCenterPos, 16, 32);
            k++;
        }
    }
}
