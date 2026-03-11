#pragma once

#include <dino/xdino.h>

class DinoTerrain {

    DinoVec2 sizeInTiles;
    DinoVec2 translation;
    int flwrNbrPerGroup;
    int season;

    uint64_t texID_terrain;
    uint64_t vbufID_bg;
    uint64_t vbufID_terrain;
    uint64_t vbufID_flwrs;

public:
    void Init(DinoVec2 sizeOfTerrainInTiles, int NbrOfFlowersPerGroup);
    void Shut();

    void DrawBG();
    void DrawTerrain();
    void DrawFlwrs();

private:
    uint64_t GenBgVertexBuffer();
    uint64_t GenTerrainVertexBuffer();
    uint64_t GenFlwrVertexBuffer(std::vector<DinoVec2> flwrPos, std::vector<DinoVec2> flwrUVs);
};