#pragma once
#include <vector>
#include <dino/xdino.h>

struct Flower {
    DinoVec2 pos;
    int type;
};

class DinoTerrain {
public:
    void Init(int tilesX = 16, int tilesY = 12, int tileSize = 16, DinoVec2 origin = {112, 84});
    void Draw(double timeSinceStart);
    void Shut();

    DinoVec2 GetOrigin() const
    {
        return terrainOrigin;
    }

    int GetWidth() const
    {
        return TILE_SIZE * TILES_X;
    }

    int GetHeight() const
    {
        return TILE_SIZE * TILES_Y;
    }

    int GetTileSize() const
    {
        return TILE_SIZE;
    }

    DinoVec2 GetRandomCellCenter() const;

private:
    uint64_t texID_terrain = 0;
    uint64_t terrainVbuf = 0;

    int season = 0;
    int biomeOffset = 0;

    int TILE_SIZE = 16;
    int TILES_X = 16;
    int TILES_Y = 12;

    DinoVec2 terrainOrigin = {112, 84};

    std::vector<Flower> flowers;

    void DrawOcean() const;
    void DrawFlowers() const;
};