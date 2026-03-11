#include "Terrain.h"

#include <iostream>


Terrain::Terrain()
{
}

void Terrain::SetUpTerrain(DinoVec2 renderSize)
{
    flowerCoords.clear();
    currentFlower = 0;
    flowerCoords.reserve(numberFlowers);

    texID_terrain = XDino_CreateGpuTexture("terrain.png");
    
    DrawOcean();
    DrawTerrain();
    DrawFlowers();
}

void Terrain::DrawOcean()
{
    {
        std::cout << "OCEAN" << std::endl;
        
        texID_ocean = XDino_CreateGpuTexture("terrain.png");
        std::vector<DinoVertex> vs;
        
        vs.resize(6);
        vs[0].pos = {-2, -1};
        vs[1].pos = {2, -1};
        vs[2].pos = {-2, 1};
        
        vs[3].pos = {2, -1};
        vs[4].pos = {-2, 1};
        vs[5].pos = {2, 1};
        
        vs[0].u = 0+ (80 * randomSaison);        vs[0].v = 0;
        vs[1].u = 0+ (80 * randomSaison);        vs[1].v = 0;
        vs[2].u = 0+ (80 * randomSaison);        vs[2].v = 0;
        vs[3].u = 0+ (80 * randomSaison);        vs[3].v = 0;
        vs[4].u = 0+ (80 * randomSaison);        vs[4].v = 0;
        vs[5].u = 0+ (80 * randomSaison);        vs[5].v = 0;
        
        vbufID_ocean = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Ocean");
    }
}

uint64_t Terrain::DrawTuilesTerrain(int offset)
{
    {
        uint64_t vbufID_terrain;
        
        std::cout << "TERRAIN" << std::endl;
        
        std::vector<DinoVertex> vs;
        
        vs.resize(6 * (24*16));

        int idVertex = 0;
        float idx = 0;
        float idy = 0;

        int x_maxiIndex = 23;
        int y_maxiIndex = 15;
        
        for (int i = 0; i < x_maxiIndex+1; i++)
        {
            for (int j = 0; j < y_maxiIndex+1; j++)
            {
                idx = i * 16;
                idy = j * 16;
                
                vs[idVertex].pos = {idx, idy};
                vs[idVertex + 1].pos = {(idx + 16), idy};
                vs[idVertex +2].pos = {idx, (idy + 16)};
        
                vs[idVertex +3].pos = {(idx + 16), idy};
                vs[idVertex +4].pos = {idx, (idy + 16)};
                vs[idVertex +5].pos = {(idx + 16), (idy + 16)};
                
                if (i == 1 && j == 1)
                {
                    _MINI_terrainBound.x = idx;
                    _MINI_terrainBound.y = idy;
                }

                if (i == x_maxiIndex -1 && j == y_maxiIndex -1 )
                {
                    _MAXI_terrainBound.x = idx;
                    _MAXI_terrainBound.y = idy;
                }
                
                if (i == 0 && j == 0)
                    DrawTuile(vs, idVertex, 0, 0, offset);
                else if (i == x_maxiIndex && j == 0)
                    DrawTuile(vs, idVertex, 32, 0, offset);
                else if (i == 0 && j == y_maxiIndex)
                    DrawTuile(vs, idVertex, 0, 32 ,offset);
                else if (i == x_maxiIndex && j == y_maxiIndex)
                    DrawTuile(vs, idVertex, 32, 32, offset);
                else if (i > 0 && i < x_maxiIndex && j == 0)
                    DrawTuile(vs, idVertex, 16, 0, offset);
                else if (i > 0 && i < x_maxiIndex && j == y_maxiIndex)
                    DrawTuile(vs, idVertex, 16, 32, offset);
                else if (j > 0 && j < y_maxiIndex && i == 0)
                    DrawTuile(vs, idVertex, 0, 16, offset);
                else if (j > 0 && j < y_maxiIndex && i == x_maxiIndex)
                    DrawTuile(vs, idVertex, 32, 16, offset);
                else
                {
                    int r = XDino_RandomInt32(0, 100);
                    if (r <= percentLuckSpawnFlower)
                    {
                        if (currentFlower < numberFlowers)
                        {
                            flowerCoords.emplace_back(idx, idy);
                        
                            currentFlower++;    
                        }
                    }                  
                    
                    
                    DrawDefaultTuile(vs, idVertex);
                }

                
                idVertex += 6;
            }
        }

        vbufID_terrain = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Terrain");
        return vbufID_terrain;
    }
}

void Terrain::DrawTuile(std::vector<DinoVertex>& vs, int idVertex, int u, int v, int offsetAnim)
{
    vs[idVertex +0].u = u + 0 + (80 * randomSaison);        vs[idVertex +0].v = v + 16 + (offsetAnim * 48);
    vs[idVertex +1].u = u + 16+ (80 * randomSaison);        vs[idVertex +1].v = v + 16+ (offsetAnim * 48);
    vs[idVertex +2].u = u +0+ (80 * randomSaison);        vs[idVertex +2].v = v + 32+ (offsetAnim * 48);
    vs[idVertex +3].u = u +16+ (80 * randomSaison);        vs[idVertex +3].v = v + 16+ (offsetAnim * 48);
    vs[idVertex +4].u = u +0+ (80 * randomSaison);        vs[idVertex +4].v = v + 32+ (offsetAnim * 48);
    vs[idVertex +5].u = u +16+ (80 * randomSaison);        vs[idVertex +5].v = v +32+ (offsetAnim * 48);
}

void Terrain::DrawDefaultTuile(std::vector<DinoVertex>& vs, int idVertex)
{
    vs[idVertex +0].u = 16+ (80 * randomSaison); vs[idVertex +0].v = 0;
    vs[idVertex +1].u = 32+ (80 * randomSaison); vs[idVertex +1].v = 0;
    vs[idVertex +2].u = 16+ (80 * randomSaison); vs[idVertex +2].v = 16;

    vs[idVertex +3].u = 32+ (80 * randomSaison); vs[idVertex +3].v = 0;
    vs[idVertex +4].u = 16+ (80 * randomSaison); vs[idVertex +4].v = 16;
    vs[idVertex +5].u = 32+ (80 * randomSaison); vs[idVertex +5].v = 16;
}

void Terrain::DrawFlowers()
{
    std::vector<DinoVertex> vs;
    vs.resize(numberFlowers * 6);

    int idVertex = 0;
    
    int flowersPerType = numberFlowers / 3;
    
    int type = 0;
    int count = 0;
    
    for (int i = 0; i < numberFlowers; i++)
    {
        flowerCoord f = flowerCoords.at(i);

        int u = 0;

        if (type == 0) u = 0;
        if (type == 1) u = 16;
        if (type == 2) u = 32;

        DrawSoloFlower(vs, idVertex, f.x, f.y, u);

        count++;

        if (count >= flowersPerType && type < 2)
        {
            type++;
            count = 0;
        }

        idVertex += 6;
    }

    buf_flower = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Flowers");
}

void Terrain::DrawSoloFlower(std::vector<DinoVertex>& vs, int idVertex, float x, float y, int u)
{
    vs[idVertex+0].pos = {x, y};
    vs[idVertex+1].pos = {x+16, y};
    vs[idVertex+2].pos = {x, y+16};

    vs[idVertex+3].pos = {x+16, y};
    vs[idVertex+4].pos = {x, y+16};
    vs[idVertex+5].pos = {x+16, y+16};

    vs[idVertex+0].u = 32+u+ (80 * randomSaison); vs[idVertex+0].v = 0;
    vs[idVertex+1].u = 48+u+ (80 * randomSaison); vs[idVertex+1].v = 0;
    vs[idVertex+2].u = 32+u+ (80 * randomSaison); vs[idVertex+2].v = 16;

    vs[idVertex+3].u = 48+u+ (80 * randomSaison); vs[idVertex+3].v = 0;
    vs[idVertex+4].u = 32+u+ (80 * randomSaison); vs[idVertex+4].v = 16;
    vs[idVertex+5].u = 48+u+ (80 * randomSaison); vs[idVertex+5].v = 16;
}

void Terrain::DrawTerrain()
{
    for (int i= 0; i < 4; i++)
    {
        framesAnim.emplace_back(DrawTuilesTerrain(i));
    }
}


void Terrain::Update(float deltaTime)
{
    _renderSize = XDino_GetRenderSize();
    
    terrainPos = {_renderSize.x / 2.f - (23 * 16 / 2.f), _renderSize.y / 2.f - (15 * 16 / 2.f)};

    XDino_Draw(vbufID_ocean, texID_ocean, {_renderSize}, oceanScale);
    ApplyAnimation(deltaTime);
    XDino_Draw(buf_flower, texID_terrain, terrainPos);
}


void Terrain::ApplyAnimation(float deltaTime)
{
    timeBetweenFrames += deltaTime;

    if (timeBetweenFrames >= 0.5)
    {
        timeBetweenFrames = 0;
        terrainAnimFrame = (terrainAnimFrame+1) % framesAnim.size();
    }
    
    XDino_Draw(framesAnim[terrainAnimFrame], texID_terrain, terrainPos);
}

void Terrain::ShutDown()
{
    XDino_DestroyGpuTexture(texID_terrain);

    for (uint64_t i = 0; i < framesAnim.size(); i++) {
        XDino_DestroyGpuTexture(framesAnim[i]);
    }

    XDino_DestroyGpuTexture(texID_ocean);
    XDino_DestroyGpuTexture(vbufID_ocean);
}