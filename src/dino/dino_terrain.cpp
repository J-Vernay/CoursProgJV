#include <dino/dino_terrain.h>

void DinoTerrain::Init(DinoVec2 rdrSize)
{
    constexpr DinoVec2 TILE_COUNT = {16, 12};
    constexpr DinoVec2 TERRAIN_SIZE = {TILE_COUNT.x * 16, TILE_COUNT.y * 16};

    float dx = (rdrSize.x - TERRAIN_SIZE.x) / 2;
    float dy = (rdrSize.y - TERRAIN_SIZE.y) / 2;

    std::vector<DinoVertex> vs;
    vs.resize(12);

    // 1er triangle océan
    vs[0].pos = {0, 0};
    vs[0].u = 0;
    vs[0].v = 0;
    vs[1].pos = {rdrSize.x, 0};
    vs[1].u = 16;
    vs[1].v = 0;
    vs[2].pos = {0, rdrSize.y};
    vs[2].u = 0;
    vs[2].v = 16;

    // 2eme triangle océan
    vs[3].pos = {rdrSize.x, 0};
    vs[3].u = 16;
    vs[3].v = 0;
    vs[4].pos = {0, rdrSize.y};
    vs[4].u = 0;
    vs[4].v = 16;
    vs[5].pos = {rdrSize.x, rdrSize.y};
    vs[5].u = 16;
    vs[5].v = 16;

    // 1er triangle terrain
    vs[6].pos = {dx, dy};
    vs[6].u = 16;
    vs[6].v = 0;
    vs[7].pos = {dx + TERRAIN_SIZE.x, dy};
    vs[7].u = 32;
    vs[7].v = 0;
    vs[8].pos = {dx, dy + TERRAIN_SIZE.y};
    vs[8].u = 16;
    vs[8].v = 16;

    // 2eme triangle terrain
    vs[9].pos = {dx + TERRAIN_SIZE.x, dy};
    vs[9].u = 32;
    vs[9].v = 0;
    vs[10].pos = {dx, dy + TERRAIN_SIZE.y};
    vs[10].u = 16;
    vs[10].v = 16;
    vs[11].pos = {dx + TERRAIN_SIZE.x, dy + TERRAIN_SIZE.y};
    vs[11].u = 32;
    vs[11].v = 16;

    m_vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Terrain");
    m_texID = XDino_CreateGpuTexture("terrain.png");
}

void DinoTerrain::Draw()
{
    XDino_Draw(m_vbufID, m_texID);
}

void DinoTerrain::Shut()
{
    XDino_DestroyVertexBuffer(m_vbufID);
    XDino_DestroyGpuTexture(m_texID);
}