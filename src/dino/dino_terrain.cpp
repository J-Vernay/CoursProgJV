#include <dino/xdino.h>
#include <dino/dino_terrain.h>
#include <algorithm>
#include <random>

constexpr DinoVec2 TILE_COUNT = {16, 16};
constexpr DinoVec2 TERRAIN_SIZE = {TILE_COUNT.x * 16, TILE_COUNT.y * 16};

void DinoTerrain::Init(DinoVec2 rdrSize, int idxSeason)
{
    m_idxSeason = idxSeason;
    float dx = (rdrSize.x - TERRAIN_SIZE.x) / 2;
    float dy = (rdrSize.y - TERRAIN_SIZE.y) / 2;

    uint64_t uBase = idxSeason * 80;

    std::vector<DinoVertex> vs;
    vs.resize(12);
    //1er triangle Ocean
    vs[0].pos = {0, 0};
    vs[0].u = uBase;
    vs[0].v = 0;
    vs[1].pos = {rdrSize.x, 0};
    vs[1].u = uBase + 16;
    vs[1].v = 0;
    vs[2].pos = {0, rdrSize.y};
    vs[2].u = uBase;
    vs[2].v = 16;
    //2 eme triangle ocean
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
    vs[6].pos = {dx, dy};
    vs[6].u = uBase + 16;
    vs[6].v = 0;
    vs[7].pos = {dx + TERRAIN_SIZE.x, dy};
    vs[7].u = uBase + 32;
    vs[7].v = 0;
    vs[8].pos = {dx, dy + TERRAIN_SIZE.y};
    vs[8].u = uBase + 16;
    vs[8].v = 16;
    // 2eme triangle terrain
    vs[9].pos = {dx + TERRAIN_SIZE.x, dy};
    vs[9].u = uBase + 32;
    vs[9].v = 0;
    vs[10].pos = {dx, dy + TERRAIN_SIZE.y};
    vs[10].u = uBase + 16;
    vs[10].v = 16;
    vs[11].pos = {dx + TERRAIN_SIZE.x, dy + TERRAIN_SIZE.y};
    vs[11].u = uBase + 32;
    vs[11].v = 16;

    m_posTopLeft = {dx, dy};
    CreateEdges(idxSeason, 0, {dx, dy});
    SpawnFlowers(vs, idxSeason, {dx, dy});
    m_vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Terrain");
    m_texID = XDino_CreateGpuTexture("terrain.png");

}

void DinoTerrain::Draw()
{
    XDino_Draw(m_vbufID, m_texID);
}

void DinoTerrain::Update(double timeSinceStart)
{
    int idxAnim = int(timeSinceStart * 2) % 4;
    uint64_t vbufID = CreateEdges(m_idxSeason, idxAnim, m_posTopLeft);
    XDino_Draw(vbufID, m_texID);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoTerrain::Shut()
{
    XDino_DestroyVertexBuffer(m_vbufID);
    XDino_DestroyGpuTexture(m_texID);
}

uint64_t DinoTerrain::CreateEdges(int idxSeason, int idxAnim, DinoVec2 posTopLeft)
{
    std::vector<DinoVertex> vs;

    for (int ty = 0; ty < TILE_COUNT.y; ty++) {
        for (int tx = 0; tx < TILE_COUNT.x; tx++) {
            if (tx != 0 && tx != TILE_COUNT.x - 1 &&
                ty != 0 && ty != TILE_COUNT.y - 1)
                continue;

            CreateEdgeTile(vs, idxSeason, posTopLeft, idxAnim, tx, ty);
        }
    }

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Edges");
}

void DinoTerrain::CreateEdgeTile(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft, int idxAnim, int tx,
                                 int ty)
{
    int i = out.size();
    out.resize(i + 6);

    float x = posTopLeft.x + tx * 16.f;
    float y = posTopLeft.y + ty * 16.f;

    bool left = tx == 0;
    bool right = tx == TILE_COUNT.x - 1;
    bool top = ty == 0;
    bool bottom = ty == TILE_COUNT.y - 1;

    int u = left ? 0 : right ? 32 : 16;
    int v = top ? 16 : bottom ? 48 : 32;

    uint16_t uBase = idxSeason * 80;
    uint16_t vBase = idxAnim * 48;

    out[i].pos = {x, y};
    out[i].u = uBase + u;
    out[i].v = v + vBase;
    out[i + 1].pos = {x + 16, y};
    out[i + 1].u = uBase + u + 16;
    out[i + 1].v = v + vBase;
    out[i + 2].pos = {x, y + 16};
    out[i + 2].u = uBase + u;
    out[i + 2].v = v + 16 + vBase;
    out[i + 3].pos = {x + 16, y};
    out[i + 3].u = uBase + u + 16;
    out[i + 3].v = v + vBase;
    out[i + 4].pos = {x, y + 16};
    out[i + 4].u = uBase + u;
    out[i + 4].v = v + 16 + vBase;
    out[i + 5].pos = {x + 16, y + 16};
    out[i + 5].u = uBase + u + 16;
    out[i + 5].v = v + 16 + vBase;

}

void DinoTerrain::SpawnFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft)
{
    std::vector<DinoVec2> posCandidates;
    for (int ty = 1; ty < TILE_COUNT.y - 1; ty++)
        for (int tx = 1; tx < TILE_COUNT.x - 1; tx++)
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
        SpawnFlower(out, idxSeason, 0, posCandidates[i]);
    for (int i = 10; i < 20; ++i)
        SpawnFlower(out, idxSeason, 1, posCandidates[i]);
    for (int i = 20; i < 30; ++i)
        SpawnFlower(out, idxSeason, 2, posCandidates[i]);

}

void DinoTerrain::SpawnFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos)
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