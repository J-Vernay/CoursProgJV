#include <dino/xdino.h>
#include <dino/dino_terrain.h>
#include <cmath>

void DinoTerrain::Init(DinoVec2 sizeOfTerrainInTiles, int NbrOfFlowersPerGroup)
{
    DinoVec2 debug = XDino_GetRenderSize();
    translation = {(XDino_GetRenderSize().x - sizeOfTerrainInTiles.x * 16) / 2,
                   (XDino_GetRenderSize().y - sizeOfTerrainInTiles.y * 16) / 2};

    flwrNbrPerGroup = NbrOfFlowersPerGroup;
    season = XDino_RandomInt32(0, 3);

    texID_terrain = XDino_CreateGpuTexture("terrain.png");

    vbufID_bg = GenBgVertexBuffer();

    sizeInTiles = sizeOfTerrainInTiles;
    vbufID_terrain = GenTerrainVertexBuffer();
}

DinoVec2 DinoTerrain::GetTopLeft()
{
    return {translation.x + 16, translation.y + 16}; // Corner tile that is not a border tile
}

DinoVec2 DinoTerrain::GetBottomRight()
{
    return {translation.x + (sizeInTiles.x - 1) * 16, translation.y + (sizeInTiles.y - 1) * 16};
    // Top left corner of Corner tile that is not a border tile 
}

void DinoTerrain::Shut()
{
    XDino_DestroyGpuTexture(texID_terrain);
    XDino_DestroyVertexBuffer(vbufID_bg);
    XDino_DestroyVertexBuffer(vbufID_terrain);
    XDino_DestroyVertexBuffer(vbufID_flwrs);
}

uint64_t DinoTerrain::GenBgVertexBuffer()
{
    DinoVec2 windowSize = XDino_GetWindowSize();

    std::vector<DinoVertex> vs;
    vs.resize(6);

    vs[0].pos = {0, 0};
    vs[0].u = 0 + season * 80;
    vs[0].v = 0;

    vs[1].pos = {windowSize.x, 0};
    vs[1].u = 16 + season * 80;
    vs[1].v = 0;

    vs[2].pos = {0, windowSize.y};
    vs[2].u = 0 + season * 80;
    vs[2].v = 16;

    vs[3].pos = {windowSize.x, 0};
    vs[3].u = 16 + season * 80;
    vs[3].v = 0;

    vs[4].pos = {windowSize.x, windowSize.y};
    vs[4].u = 16 + season * 80;
    vs[4].v = 16;

    vs[5].pos = {0, windowSize.y};
    vs[5].u = 0 + season * 80;
    vs[5].v = 16;

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "background");
}

void DinoTerrain::DrawBG()
{
    XDino_Draw(vbufID_bg, texID_terrain, {0, 0});
}

uint64_t DinoTerrain::GenTerrainVertexBuffer()
{
    std::vector<DinoVec2> possiblePos;
    possiblePos.resize(sizeInTiles.x * sizeInTiles.y);

    for (int i = 0; i < sizeInTiles.x * sizeInTiles.y; i++) {
        float intPart;
        modf(i / sizeInTiles.x, &intPart);

        possiblePos[i] = {(float)(i % (int)sizeInTiles.x), intPart};
    }

    DinoVec2 uvBase;
    std::vector<DinoVertex> vs;
    vs.resize(6 * sizeInTiles.x * sizeInTiles.y);

    std::vector<DinoVec2> flwrPos;
    flwrPos.resize(flwrNbrPerGroup * 3);
    std::vector<DinoVec2> flwrUVs;
    flwrUVs.resize(flwrNbrPerGroup * 3);
    int flwr1Count = flwrNbrPerGroup;
    int flwr2Count = flwrNbrPerGroup;
    int flwr3Count = flwrNbrPerGroup;

    for (int i = 0; i < sizeInTiles.x * sizeInTiles.y; i++) {
        int indexOfTileToChange = XDino_RandomInt32(i, sizeInTiles.x * sizeInTiles.y - 1);
        DinoVec2 tileToChange = possiblePos[indexOfTileToChange];

        possiblePos[indexOfTileToChange] = possiblePos[i];
        possiblePos[i] = tileToChange;

        // UV Base
        {
            if (possiblePos[i].x == 0 && possiblePos[i].y == 0) {
                uvBase = {0, 16};
            }
            else if (possiblePos[i].x == 0 && possiblePos[i].y == sizeInTiles.y - 1) {
                uvBase = {0, 48};
            }
            else if (possiblePos[i].x == sizeInTiles.x - 1 && possiblePos[i].y == 0) {
                uvBase = {32, 16};
            }
            else if (possiblePos[i].x == sizeInTiles.x - 1 && possiblePos[i].y == sizeInTiles.y - 1) {
                uvBase = {32, 48};
            }
            else if (possiblePos[i].x == 0) {
                uvBase = {0, 32};
            }
            else if (possiblePos[i].x == sizeInTiles.x - 1) {
                uvBase = {32, 32};
            }
            else if (possiblePos[i].y == 0) {
                uvBase = {16, 16};
            }
            else if (possiblePos[i].y == sizeInTiles.y - 1) {
                uvBase = {16, 48};
            }
            else if (flwr1Count > 0) {
                flwrPos[30 - (flwr1Count + flwr2Count + flwr3Count)] = possiblePos[i];
                flwrUVs[30 - (flwr1Count + flwr2Count + flwr3Count)] = {32, 0};
                flwr1Count--;
                uvBase = {16, 0};
            }
            else if (flwr2Count > 0) {
                flwrPos[30 - (flwr1Count + flwr2Count + flwr3Count)] = possiblePos[i];
                flwrUVs[30 - (flwr1Count + flwr2Count + flwr3Count)] = {48, 0};
                flwr2Count--;
                uvBase = {16, 0};
            }
            else if (flwr3Count > 0) {
                flwrPos[30 - (flwr1Count + flwr2Count + flwr3Count)] = possiblePos[i];
                flwrUVs[30 - (flwr1Count + flwr2Count + flwr3Count)] = {64, 0};
                flwr3Count--;
                uvBase = {16, 0};
            }
            else {
                uvBase = {16, 0};
            }
        }

        // VS
        {
            vs[0 + i * 6].pos = {0 + possiblePos[i].x * 16, 0 + possiblePos[i].y * 16};
            vs[0 + i * 6].u = 0 + uvBase.x + season * 80;
            vs[0 + i * 6].v = 0 + uvBase.y;

            vs[1 + i * 6].pos = {16 + possiblePos[i].x * 16, 0 + possiblePos[i].y * 16};
            vs[1 + i * 6].u = 16 + uvBase.x + season * 80;
            vs[1 + i * 6].v = 0 + uvBase.y;

            vs[2 + i * 6].pos = {0 + possiblePos[i].x * 16, 16 + possiblePos[i].y * 16};
            vs[2 + i * 6].u = 0 + uvBase.x + season * 80;
            vs[2 + i * 6].v = 16 + uvBase.y;

            vs[3 + i * 6].pos = {16 + possiblePos[i].x * 16, 0 + possiblePos[i].y * 16};
            vs[3 + i * 6].u = 16 + uvBase.x + season * 80;
            vs[3 + i * 6].v = 0 + uvBase.y;

            vs[4 + i * 6].pos = {16 + possiblePos[i].x * 16, 16 + possiblePos[i].y * 16};
            vs[4 + i * 6].u = 16 + uvBase.x + season * 80;
            vs[4 + i * 6].v = 16 + uvBase.y;

            vs[5 + i * 6].pos = {0 + possiblePos[i].x * 16, 16 + possiblePos[i].y * 16};
            vs[5 + i * 6].u = 0 + uvBase.x + season * 80;
            vs[5 + i * 6].v = 16 + uvBase.y;
        }
    }
    vbufID_terrain = XDino_CreateVertexBuffer(vs.data(), vs.size(), "terrain");
    vbufID_flwrs = GenFlwrVertexBuffer(flwrPos, flwrUVs);
    return vbufID_terrain;
}

void DinoTerrain::DrawTerrain()
{
    XDino_Draw(vbufID_terrain, texID_terrain, translation);
}

uint64_t DinoTerrain::GenFlwrVertexBuffer(std::vector<DinoVec2> flwrPos, std::vector<DinoVec2> flwrUVs)
{
    std::vector<DinoVertex> vs;
    vs.resize(6 * flwrPos.size());

    for (int i = 0; i < flwrPos.size(); i++) {
        vs[0 + i * 6].pos = {0 + flwrPos[i].x * 16, 0 + flwrPos[i].y * 16};
        vs[0 + i * 6].u = 0 + flwrUVs[i].x + season * 80;
        vs[0 + i * 6].v = 0 + flwrUVs[i].y;

        vs[1 + i * 6].pos = {16 + flwrPos[i].x * 16, 0 + flwrPos[i].y * 16};
        vs[1 + i * 6].u = 16 + flwrUVs[i].x + season * 80;
        vs[1 + i * 6].v = 0 + flwrUVs[i].y;

        vs[2 + i * 6].pos = {0 + flwrPos[i].x * 16, 16 + flwrPos[i].y * 16};
        vs[2 + i * 6].u = 0 + flwrUVs[i].x + season * 80;
        vs[2 + i * 6].v = 16 + flwrUVs[i].y;

        vs[3 + i * 6].pos = {16 + flwrPos[i].x * 16, 0 + flwrPos[i].y * 16};
        vs[3 + i * 6].u = 16 + flwrUVs[i].x + season * 80;
        vs[3 + i * 6].v = 0 + flwrUVs[i].y;

        vs[4 + i * 6].pos = {16 + flwrPos[i].x * 16, 16 + flwrPos[i].y * 16};
        vs[4 + i * 6].u = 16 + flwrUVs[i].x + season * 80;
        vs[4 + i * 6].v = 16 + flwrUVs[i].y;

        vs[5 + i * 6].pos = {0 + flwrPos[i].x * 16, 16 + flwrPos[i].y * 16};
        vs[5 + i * 6].u = 0 + flwrUVs[i].x + season * 80;
        vs[5 + i * 6].v = 16 + flwrUVs[i].y;
    }

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "flowers");
}

void DinoTerrain::DrawFlwrs()
{
    XDino_Draw(vbufID_flwrs, texID_terrain, translation);
}