#include <dino/xdino.h>
#include <dino/dino_terrain.h>
#include <cmath>

void DinoTerrain::Init()
{
    texID_terrain = XDino_CreateGpuTexture("terrain.png");
    vbufID_bg = GenBgVertexBuffer();
}

void DinoTerrain::Shut()
{
    XDino_DestroyGpuTexture(texID_terrain);
    XDino_DestroyVertexBuffer(vbufID_bg);
}

uint64_t DinoTerrain::GenBgVertexBuffer()
{
    DinoVec2 windowSize = XDino_GetWindowSize();

    std::vector<DinoVertex> vs;
    vs.resize(6);

    vs[0].pos = {0, 0};
    vs[0].u = 0;
    vs[0].v = 0;

    vs[1].pos = {windowSize.x, 0};
    vs[1].u = 16;
    vs[1].v = 0;

    vs[2].pos = {0, windowSize.y};
    vs[2].u = 0;
    vs[2].v = 16;

    vs[3].pos = {windowSize.x, 0};
    vs[3].u = 16;
    vs[3].v = 0;

    vs[4].pos = {windowSize.x, windowSize.y};
    vs[4].u = 16;
    vs[4].v = 16;

    vs[5].pos = {0, windowSize.y};
    vs[5].u = 0;
    vs[5].v = 16;

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "background");
}

void DinoTerrain::DrawBG()
{
    XDino_Draw(vbufID_bg, texID_terrain, {0, 0});
}

uint64_t DinoTerrain::GenTerrainVertexBuffer(DinoVec2 sizeInTiles)
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
            else {
                uvBase = {16, 0};
            }
        }

        // VS
        {
            float intPart;
            modf(i / sizeInTiles.x, &intPart);

            vs[0 + i * 6].pos = {0 + possiblePos[i].x * 16, 0 + possiblePos[i].y * 16};
            vs[0 + i * 6].u = 0 + uvBase.x;
            vs[0 + i * 6].v = 0 + uvBase.y;

            vs[1 + i * 6].pos = {16 + possiblePos[i].x * 16, 0 + possiblePos[i].y * 16};
            vs[1 + i * 6].u = 16 + uvBase.x;
            vs[1 + i * 6].v = 0 + uvBase.y;

            vs[2 + i * 6].pos = {0 + possiblePos[i].x * 16, 16 + possiblePos[i].y * 16};
            vs[2 + i * 6].u = 0 + uvBase.x;
            vs[2 + i * 6].v = 16 + uvBase.y;

            vs[3 + i * 6].pos = {16 + possiblePos[i].x * 16, 0 + possiblePos[i].y * 16};
            vs[3 + i * 6].u = 16 + uvBase.x;
            vs[3 + i * 6].v = 0 + uvBase.y;

            vs[4 + i * 6].pos = {16 + possiblePos[i].x * 16, 16 + possiblePos[i].y * 16};
            vs[4 + i * 6].u = 16 + uvBase.x;
            vs[4 + i * 6].v = 16 + uvBase.y;

            vs[5 + i * 6].pos = {0 + possiblePos[i].x * 16, 16 + possiblePos[i].y * 16};
            vs[5 + i * 6].u = 0 + uvBase.x;
            vs[5 + i * 6].v = 16 + uvBase.y;
        }
    }

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "terrain");
}

void DinoTerrain::DrawTerrain(DinoVec2 sizeInTiles)
{
    vbufID_terrain = GenTerrainVertexBuffer(sizeInTiles);
    XDino_Draw(vbufID_terrain, texID_terrain, {112, 84});
    XDino_DestroyVertexBuffer(vbufID_terrain);
}