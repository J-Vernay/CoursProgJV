#pragma once

#include <dino/xdino.h>

class DinoTerrain {

    uint64_t vbufID_bg;
    uint64_t vbufID_terrain;
    uint64_t texID_terrain;

public:
    void Init();
    void Shut();

    void DrawBG();
    void DrawTerrain(DinoVec2 sizeInTiles);

private:
    uint64_t GenBgVertexBuffer();
    uint64_t GenTerrainVertexBuffer(DinoVec2 sizeInTiles);
};