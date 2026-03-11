#pragma once

#include <dino/xdino.h>

class dino_terrain {
public :
    DinoVec2 DinoTerrain_Init(uint64_t textIdTerrain, int season);
    void DinoTerrain_Draw();
    void DinoTerrain_ShutDown();

private :
    uint64_t textIdTerrain;

    uint64_t vbufID_ocean;
    uint64_t vbufID_terrain;
    uint64_t vbufID_flowers;

    int seasonId;

    void GenerateTerrainBuffer(std::vector<DinoVertex>& oceanVector, std::vector<DinoVertex>& terrainVector);
    void GenerateFlowerBuffer(std::vector<DinoVertex>& flowerVector);
    void GenerateFlowerAtPos(DinoVec2 pos, std::vector<DinoVertex>& flowerVector, int flowerIndex);
    void GetFullPositionGrid(std::vector<DinoVec2>& posVector);
};