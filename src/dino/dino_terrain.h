#pragma once

#include <dino/xdino.h>

class dino_terrain {
public :
    dino_terrain(int season);
    dino_terrain(dino_terrain const& other) = delete;
    ~dino_terrain();

    dino_terrain& operator=(dino_terrain const& other) = delete;

    void DinoTerrain_Draw();

private :
    uint64_t textIdTerrain;

    uint64_t vbufID_ocean;
    uint64_t vbufID_terrain;
    uint64_t vbufID_flowers;

    int seasonId;

    void GenerateFullTerrain(int season);

    void GenerateTerrainBuffer(std::vector<DinoVertex>& oceanVector, std::vector<DinoVertex>& terrainVector);
    void GenerateFlowerBuffer(std::vector<DinoVertex>& flowerVector);
    void GenerateFlowerAtPos(DinoVec2 pos, std::vector<DinoVertex>& flowerVector, int flowerIndex);
    void GetFullPositionGrid(std::vector<DinoVec2>& posVector);
};