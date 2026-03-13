#include "Terrain.h"

#include "dino/xdino.h"

#include <iostream>


Terrain::Terrain() = default;

void Terrain::SetUpTerrain()
{
    flowerCoords.clear();
    currentFlower = 0;
    flowerCoords.reserve(numberFlowers);

    DinoVec2 renderSize = XDino_GetRenderSize();
    
    oceanScale = renderSize.x / 2 + renderSize.y / 2;

    texID_terrain = XDino_CreateGpuTexture("terrain.png");

    randomSaison = XDino_RandomInt32(0, 3);
    
    DrawOcean();
    DrawTerrain();
    DrawFlowers();
}

void Terrain::DrawOcean()
{
    texID_ocean = texID_terrain;

    DinoArray<DinoVertex> vs(0, 6);

    DinoVec2 renderSize = XDino_GetRenderSize();

    DinoVertex v0{}; v0.pos = {0,0}; 
    DinoVertex v1{}; v1.pos = {renderSize.x,0};
    DinoVertex v2{}; v2.pos = {0, renderSize.y};
    DinoVertex v3{}; v3.pos = {renderSize.x,0};
    DinoVertex v4{}; v4.pos = {0, renderSize.y};
    DinoVertex v5{}; v5.pos = {renderSize.x, renderSize.y};

    v0.u = 0 + (80 * randomSaison); v0.v = 0;
    v1.u = 16 + (80 * randomSaison); v1.v = 0;
    v2.u = 0 + (80 * randomSaison); v2.v = 16;
    v3.u = 16 + (80 * randomSaison); v3.v = 0;
    v4.u = 0 + (80 * randomSaison); v4.v = 16;
    v5.u = 16 + (80 * randomSaison); v5.v = 16;

    vs.AddBack(v0);
    vs.AddBack(v1);
    vs.AddBack(v2);
    vs.AddBack(v3);
    vs.AddBack(v4);
    vs.AddBack(v5);

    vbufID_ocean = XDino_CreateVertexBuffer(vs.data(), vs.GetSize(), "Ocean");
}

uint64_t Terrain::DrawTuilesTerrain(int offset, bool collectFlowers)
{
    uint64_t vbufID_terrain;
    
    std::vector<DinoVertex> vs(6 * (24*16));
    
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
            vs[idVertex + 2].pos = {idx, (idy + 16)};
    
            vs[idVertex + 3].pos = {(idx + 16), idy};
            vs[idVertex + 4].pos = {idx, (idy + 16)};
            vs[idVertex + 5].pos = {(idx + 16), (idy + 16)};
            
            if (i == 1 && j == 1)
            {
                _MINI_terrainBound.x = idx;
                _MINI_terrainBound.y = idy;
            }

            if (i == x_maxiIndex - 1 && j == y_maxiIndex - 1)
            {
                _MAXI_terrainBound.x = idx;
                _MAXI_terrainBound.y = idy;
            }
            
            if (i == 0 && j == 0)
                DrawTuile(vs, idVertex, 0, 0, offset);
            else if (i == x_maxiIndex && j == 0)
                DrawTuile(vs, idVertex, 32, 0, offset);
            else if (i == 0 && j == y_maxiIndex)
                DrawTuile(vs, idVertex, 0, 32, offset);
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
                if (collectFlowers)
                {
                    int r = XDino_RandomInt32(0, 100);
                    if (r <= percentLuckSpawnFlower && currentFlower < numberFlowers)
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
    if (currentFlower == 0) return;

    DinoArray<DinoVertex> vs(0, currentFlower * 6); // capacité = nombre de vertices

    int flowersPerType = currentFlower / 3;
    int type = 0;
    int count = 0;

    for (int i = 0; i < currentFlower; i++)
    {
        flowerCoord f = flowerCoords.at(i);

        int u = type * 16;

        // Crée 6 vertices pour une fleur
        for (int j = 0; j < 6; j++)
        {
            DinoVertex vertex{};
            switch (j)
            {
            case 0: vertex.pos = {f.x, f.y}; vertex.u = 32 + u + (80 * randomSaison); vertex.v = 0; break;
            case 1: vertex.pos = {f.x + 16, f.y}; vertex.u = 48 + u + (80 * randomSaison); vertex.v = 0; break;
            case 2: vertex.pos = {f.x, f.y + 16}; vertex.u = 32 + u + (80 * randomSaison); vertex.v = 16; break;
            case 3: vertex.pos = {f.x + 16, f.y}; vertex.u = 48 + u + (80 * randomSaison); vertex.v = 0; break;
            case 4: vertex.pos = {f.x, f.y + 16}; vertex.u = 32 + u + (80 * randomSaison); vertex.v = 16; break;
            case 5: vertex.pos = {f.x + 16, f.y + 16}; vertex.u = 48 + u + (80 * randomSaison); vertex.v = 16; break;
            }
            vs.AddBack(vertex);
        }

        count++;
        if (count >= flowersPerType && type < 2)
        {
            type++;
            count = 0;
        }
    }

    buf_flower = XDino_CreateVertexBuffer(vs.data(), vs.GetSize(), "Flowers");
}

void Terrain::DrawSoloFlower(DinoArray<DinoVertex>& vs, int idVertex, float x, float y, int u)
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
    for (int i = 0; i < 4; i++)
    {
        uint64_t id = DrawTuilesTerrain(i, i == 0);
        framesAnim.emplace_back(id);
    }
}


void Terrain::Update(float deltaTime)
{
    if (framesAnim.empty()) return;
    
    _renderSize = XDino_GetRenderSize();
    
    terrainPos = {_renderSize.x / 2.f - (23 * 16 / 2.f), _renderSize.y / 2.f - (15 * 16 / 2.f)};
    
    XDino_Draw(vbufID_ocean, texID_ocean);
    
    ApplyAnimation(deltaTime);
    if (buf_flower != 0)
        XDino_Draw(buf_flower, texID_terrain, terrainPos);}


void Terrain::ApplyAnimation(float deltaTime)
{
    timeBetweenFrames += deltaTime;
    if (timeBetweenFrames >= 0.5)
    {
        timeBetweenFrames = 0;
        terrainAnimFrame = (terrainAnimFrame+1) % framesAnim.size();
        std::cout << (terrainAnimFrame+1) % framesAnim.size() << std::endl;
    }

    
    XDino_Draw(framesAnim[terrainAnimFrame], texID_terrain, terrainPos);
}

void Terrain::ShutDown()
{
    XDino_DestroyVertexBuffer(vbufID_ocean);

    XDino_DestroyGpuTexture(texID_terrain);
    
    for (size_t i = 0; i < framesAnim.size(); i++)
        XDino_DestroyVertexBuffer(framesAnim[i]);

    
    if (buf_flower != 0)
        XDino_DestroyVertexBuffer(buf_flower);
}