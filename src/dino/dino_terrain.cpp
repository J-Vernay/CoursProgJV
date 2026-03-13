#include "dino_terrain.h"

dino_terrain::dino_terrain(int season)
{
    textIdTerrain = XDino_CreateGpuTexture("terrain.png");
    GenerateFullTerrain(season);
}

dino_terrain::~dino_terrain()
{
    XDino_DestroyGpuTexture(textIdTerrain);

    XDino_DestroyVertexBuffer(vbufID_ocean);
    XDino_DestroyVertexBuffer(vbufID_terrain);
    XDino_DestroyVertexBuffer(vbufID_flowers);
}

void dino_terrain::DinoTerrain_Draw()
{
    XDino_Draw(vbufID_ocean, textIdTerrain);
    XDino_Draw(vbufID_terrain, textIdTerrain);
    XDino_Draw(vbufID_flowers, textIdTerrain);
}

void dino_terrain::GenerateFullTerrain(int season)
{
    seasonId = season;

    std::vector<DinoVertex> terrainVector;
    std::vector<DinoVertex> oceanVector;
    GenerateTerrainBuffer(oceanVector, terrainVector);

    std::vector<DinoVertex> flowerVector;
    GenerateFlowerBuffer(flowerVector);

    vbufID_ocean = XDino_CreateVertexBuffer(oceanVector.data(), oceanVector.size(), "ocean");
    vbufID_terrain = XDino_CreateVertexBuffer(terrainVector.data(), terrainVector.size(), "terrain");

    vbufID_flowers = XDino_CreateVertexBuffer(flowerVector.data(), flowerVector.size(), "flowers");
}

void dino_terrain::GenerateTerrainBuffer(std::vector<DinoVertex>& oceanVector, std::vector<DinoVertex>& terrainVector)
{
    float Uu = 80 * seasonId;
    float Uv = 0;

    //Generating ocean
    {
        oceanVector.resize(6);
        oceanVector[0].pos = {0, 0};
        oceanVector[0].color = DinoColor_WHITE;
        oceanVector[1].pos = {480, 0};
        oceanVector[1].color = DinoColor_WHITE;
        oceanVector[2].pos = {0, 360};
        oceanVector[2].color = DinoColor_WHITE;
        oceanVector[3].pos = {480, 0};
        oceanVector[3].color = DinoColor_WHITE;
        oceanVector[4].pos = {0, 360};
        oceanVector[4].color = DinoColor_WHITE;
        oceanVector[5].pos = {480, 360};
        oceanVector[5].color = DinoColor_WHITE;
        oceanVector[0].u = Uu;
        oceanVector[0].v = Uv;
        oceanVector[1].u = Uu + 16;
        oceanVector[1].v = Uv;
        oceanVector[2].u = Uu;
        oceanVector[2].v = Uv + 16;
        oceanVector[3].u = Uu + 16;
        oceanVector[3].v = Uv;
        oceanVector[4].u = Uu;
        oceanVector[4].v = Uv + 16;
        oceanVector[5].u = Uu + 16;
        oceanVector[5].v = Uv + 16;
    }

    //size = 16 * 12 * 6 => terrain dimension by vertex in each tile
    terrainVector.resize(1152);
    for (int i = 0; i < terrainVector.size(); i += 6) {
        int currentX = (i / 6) % 16;
        int currentY = (i / 6) / 16;
        float Dx = (480 - 256) / 2 + (currentX * 16);
        float Dy = (360 - 192) / 2 + (currentY * 16);

        float Uu2 = 80 * seasonId;
        float Uv2 = 16;

        //if x is 0 stay on top row on texture (+0) // if x is 15 go to last row on texture (+32) // else go to middle row on texture (+16)
        Uu2 += currentX == 0 ? 0 : currentX == 15 ? 32 : 16;
        //if y is 0 stay on left column on texture (+0) // if y is 11 go to last column on texture (+32) // else go to middle column on texture (+16)
        Uv2 += currentY == 0 ? 0 : currentY == 11 ? 32 : 16;

        terrainVector[i].pos = {Dx, Dy};
        terrainVector[i].color = DinoColor_WHITE;
        terrainVector[i + 1].pos = {Dx + 16, Dy};
        terrainVector[i + 1].color = DinoColor_WHITE;
        terrainVector[i + 2].pos = {Dx, Dy + 16};
        terrainVector[i + 2].color = DinoColor_WHITE;
        terrainVector[i + 3].pos = {Dx + 16, Dy};
        terrainVector[i + 3].color = DinoColor_WHITE;
        terrainVector[i + 4].pos = {Dx, Dy + 16};
        terrainVector[i + 4].color = DinoColor_WHITE;
        terrainVector[i + 5].pos = {Dx + 16, Dy + 16};
        terrainVector[i + 5].color = DinoColor_WHITE;
        terrainVector[i].u = Uu2;
        terrainVector[i].v = Uv2;
        terrainVector[i + 1].u = Uu2 + 16;
        terrainVector[i + 1].v = Uv2;
        terrainVector[i + 2].u = Uu2;
        terrainVector[i + 2].v = Uv2 + 16;
        terrainVector[i + 3].u = Uu2 + 16;
        terrainVector[i + 3].v = Uv2;
        terrainVector[i + 4].u = Uu2;
        terrainVector[i + 4].v = Uv2 + 16;
        terrainVector[i + 5].u = Uu2 + 16;
        terrainVector[i + 5].v = Uv2 + 16;
    }
}

void dino_terrain::GenerateFlowerBuffer(std::vector<DinoVertex>& flowerVector)
{
    std::vector<DinoVec2> posVector;
    GetFullPositionGrid(posVector);
    for (int i = 0; i < 10; i++) {
        if (posVector.size() <= 0)
            break;

        int choosentIndex = XDino_RandomInt32(0, posVector.size() - 1);
        GenerateFlowerAtPos(posVector[choosentIndex], flowerVector, 0);
        posVector.erase(posVector.begin() + choosentIndex);
    }
    for (int i = 0; i < 10; i++) {
        if (posVector.size() <= 0)
            break;

        int choosentIndex = XDino_RandomInt32(0, posVector.size() - 1);
        GenerateFlowerAtPos(posVector[choosentIndex], flowerVector, 1);
        posVector.erase(posVector.begin() + choosentIndex);
    }
    for (int i = 0; i < 10; i++) {
        if (posVector.size() <= 0)
            break;

        int choosentIndex = XDino_RandomInt32(0, posVector.size() - 1);
        GenerateFlowerAtPos(posVector[choosentIndex], flowerVector, 2);
        posVector.erase(posVector.begin() + choosentIndex);
    }
}

void dino_terrain::GenerateFlowerAtPos(DinoVec2 pos, std::vector<DinoVertex>& flowerVector, int flowerIndex)
{
    float Ut = 16 + (16 * flowerIndex) + (80 * seasonId);

    int size = flowerVector.size();

    flowerVector.resize(flowerVector.size() + 6);
    flowerVector[size].pos = {pos.x, pos.y};
    flowerVector[size].color = DinoColor_WHITE;
    flowerVector[size + 1].pos = {pos.x + 16, pos.y};
    flowerVector[size + 1].color = DinoColor_WHITE;
    flowerVector[size + 2].pos = {pos.x, pos.y + 16};
    flowerVector[size + 2].color = DinoColor_WHITE;
    flowerVector[size + 3].pos = {pos.x + 16, pos.y};
    flowerVector[size + 3].color = DinoColor_WHITE;
    flowerVector[size + 4].pos = {pos.x, pos.y + 16};
    flowerVector[size + 4].color = DinoColor_WHITE;
    flowerVector[size + 5].pos = {pos.x + 16, pos.y + 16};
    flowerVector[size + 5].color = DinoColor_WHITE;
    flowerVector[size].u = Ut + 16;
    flowerVector[size].v = 0;
    flowerVector[size + 1].u = Ut + 32;
    flowerVector[size + 1].v = 0;
    flowerVector[size + 2].u = Ut + 16;
    flowerVector[size + 2].v = 16;
    flowerVector[size + 3].u = Ut + 32;
    flowerVector[size + 3].v = 0;
    flowerVector[size + 4].u = Ut + 16;
    flowerVector[size + 4].v = 16;
    flowerVector[size + 5].u = Ut + 32;
    flowerVector[size + 5].v = 16;
}

void dino_terrain::GetFullPositionGrid(std::vector<DinoVec2>& posVector)
{
    float Dx = (480 - 256) / 2;
    float Dy = (360 - 192) / 2;

    posVector.resize(192);
    //preventing spawn on first and last rows
    for (int y = 1; y < 11; y++) {
        for (int x = 1; x < 15; x++) {
            posVector[y * 16 + x] = {Dx + (float)x * 16, Dy + (float)y * 16};
        }
    }
}