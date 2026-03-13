#pragma once
#include "dino/xdino.h"
#include <dino/DinoArray.h>


struct flowerCoord {
    float x;
    float y;
};

class Terrain
{
private:
    uint64_t texID_ocean;
    uint64_t vbufID_ocean;

    uint64_t texID_terrain = 0;
    std::vector<uint64_t> framesAnim;

    DinoVec2 _renderSize;
    
    DinoVec2 oceanPos;
    DinoVec2 terrainPos;
    double terrainScale;
    double oceanScale;

    int terrainAnimFrame=0;
    float timeBetweenFrames = 0;

    int randomSaison;

    //Flower
    int numberFlowers = 30;
    int currentFlower = 0;
    int percentLuckSpawnFlower = 10;

    std::vector<flowerCoord> flowerCoords;

    uint64_t buf_flower = 0;

public:
    Terrain();
    
    void SetUpTerrain();
    void DrawOcean();
    void DrawTerrain();
    uint64_t DrawTuilesTerrain(int offset, bool collectFlowers = false);
    void DrawTuile(std::vector<DinoVertex>& vs, int idVertex, int u, int v, int offsetAnim);
    void DrawFlowers();
    void DrawSoloFlower(DinoArray<DinoVertex>& vs, int idVertex, float x, float y, int u);
    void DrawDefaultTuile(std::vector<DinoVertex>& vs, int idVertex);
    
    void ApplyAnimation(float deltaTime);
    
    void Update(float deltaTime);
    void ShutDown();

    DinoVec2 _MINI_terrainBound = {};
    DinoVec2 _MAXI_terrainBound = {};
};
