#include <dino/dino_terrain.h>

constexpr DinoVec2 TILE_COUNT = {16, 12};
constexpr DinoVec2 TERRAIN_SIZE = {TILE_COUNT.x * 16, TILE_COUNT.y * 16};

void DinoTerrain::Init(DinoVec2 rdrSize, int idxSeason)
{
    m_dx = (rdrSize.x - TERRAIN_SIZE.x) / 2;
    m_dy = (rdrSize.y - TERRAIN_SIZE.y) / 2;

    uint16_t uBase = idxSeason * 80;

    std::vector<DinoVertex> vs;
    vs.resize(12);

    // 1er triangle océan
    vs[0].pos = {0, 0};
    vs[0].u = uBase;
    vs[0].v = 0;
    vs[1].pos = {rdrSize.x, 0};
    vs[1].u = uBase + 16;
    vs[1].v = 0;
    vs[2].pos = {0, rdrSize.y};
    vs[2].u = uBase;
    vs[2].v = 16;

    // 2eme triangle océan
    vs[3].pos = {rdrSize.x, 0};
    vs[3].u = uBase + 16;
    vs[3].v = 0;
    vs[4].pos = {0, rdrSize.y};
    vs[4].u = uBase;
    vs[4].v = 16;
    vs[5].pos = {rdrSize.x, rdrSize.y};
    vs[5].u = uBase + 16;
    vs[5].v = 16;

    // 1er triangle terrain
    vs[6].pos = {m_dx, m_dy};
    vs[6].u = uBase + 16;
    vs[6].v = 0;
    vs[7].pos = {m_dx + TERRAIN_SIZE.x, m_dy};
    vs[7].u = uBase + 32;
    vs[7].v = 0;
    vs[8].pos = {m_dx, m_dy + TERRAIN_SIZE.y};
    vs[8].u = uBase + 16;
    vs[8].v = 16;

    // 2eme triangle terrain
    vs[9].pos = {m_dx + TERRAIN_SIZE.x, m_dy};
    vs[9].u = uBase + 32;
    vs[9].v = 0;
    vs[10].pos = {m_dx, m_dy + TERRAIN_SIZE.y};
    vs[10].u = uBase + 16;
    vs[10].v = 16;
    vs[11].pos = {m_dx + TERRAIN_SIZE.x, m_dy + TERRAIN_SIZE.y};
    vs[11].u = uBase + 32;
    vs[11].v = 16;

    GenFlowers(vs, idxSeason, {m_dx, m_dy});

    m_vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Terrain");
    m_texID = XDino_CreateGpuTexture("terrain.png");
}

DinoVec2 DinoTerrain::GetTopLeft()
{
    return {m_dx, m_dy};
}

DinoVec2 DinoTerrain::GetBottomRight()
{
    return {m_dx + TERRAIN_SIZE.x, m_dy + TERRAIN_SIZE.y};
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

void DinoTerrain::GenFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft)
{
    // 1. Générer toutes les positions possibles.
    std::vector<DinoVec2> posCandidates;
    for (int ty = 0; ty < TILE_COUNT.y; ty++)
        for (int tx = 0; tx < TILE_COUNT.x; tx++)
            posCandidates.push_back({posTopLeft.x + 16.f * tx, posTopLeft.y + 16.f * ty});

    // 2. Mélanger le tableau.
    for (int i = 0; i < posCandidates.size(); i++) {
        int j = XDino_RandomInt32(i, posCandidates.size() - 1);
        DinoVec2 temp = posCandidates[i];
        posCandidates[i] = posCandidates[j];
        posCandidates[j] = temp;
    }

    // 3. Générer les sommets.
    for (int i = 0; i < 10; ++i)
        GenFlower(out, idxSeason, 0, posCandidates[i]);
    for (int i = 10; i < 20; ++i)
        GenFlower(out, idxSeason, 1, posCandidates[i]);
    for (int i = 20; i < 30; ++i)
        GenFlower(out, idxSeason, 2, posCandidates[i]);
}

void DinoTerrain::GenFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos)
{
    int i = out.size();
    out.resize(i + 6);

    uint16_t uBase = idxSeason * 80 + idxFlower * 16 + 32;
    out[i].pos = {pos.x, pos.y};
    out[i].u = uBase;
    out[i].v = 0;
    out[i + 1].pos = {pos.x + 16, pos.y};
    out[i + 1].u = uBase + 16;
    out[i + 1].v = 0;
    out[i + 2].pos = {pos.x, pos.y + 16};
    out[i + 2].u = uBase;
    out[i + 2].v = 16;

    out[i + 3].pos = {pos.x + 16, pos.y};
    out[i + 3].u = uBase + 16;
    out[i + 3].v = 0;
    out[i + 4].pos = {pos.x, pos.y + 16};
    out[i + 4].u = uBase;
    out[i + 4].v = 16;
    out[i + 5].pos = {pos.x + 16, pos.y + 16};
    out[i + 5].u = uBase + 16;
    out[i + 5].v = 16;
}