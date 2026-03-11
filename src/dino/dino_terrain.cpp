#include <dino/dino_terrain.h>
#include <algorithm>
#include <random>

constexpr DinoVec2 TILE_COUNT = {16, 12};
constexpr DinoVec2 TERRAIN_SIZE = {TILE_COUNT.x * 16, TILE_COUNT.y * 16};

void DinoTerrain::Init(DinoVec2 rdrSize, int idxSeason)
{
    m_idxSeason = idxSeason;
    float dx = (rdrSize.x - TERRAIN_SIZE.x) / 2;
    float dy = (rdrSize.y - TERRAIN_SIZE.y) / 2;

    uint16_t uBase = idxSeason * 80;
    std::vector<DinoVertex> vs;
    vs.resize(12);

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

    m_topLeft = {dx, dy};
    DrawFlowers(vs, idxSeason, {dx, dy});

    m_vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Terrain");
    m_texID = XDino_CreateGpuTexture("terrain.png");

}

void DinoTerrain::Draw()
{
    XDino_Draw(m_vbufID, m_texID);
}

void DinoTerrain::Update(double timeSinceStart)
{
    int idxAnim = static_cast<int>(timeSinceStart * 2) % 4;
    uint64_t vbufID = CreateEgdes(m_idxSeason, idxAnim, m_topLeft);
    XDino_Draw(vbufID, m_texID);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoTerrain::Shut()
{
    XDino_DestroyVertexBuffer(m_vbufID);
    XDino_DestroyGpuTexture(m_texID);
}


void DinoTerrain::DrawFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft)
{
    std::vector<DinoVec2> posCandidate;
    for (int i = 0; i < TILE_COUNT.y; i++) {
        for (int j = 0; j < TILE_COUNT.x; j++) {
            posCandidate.push_back({posTopLeft.x + 16.f * j, posTopLeft.y + 16.f * i});
        }
    }

    for (int i = 0; i < posCandidate.size(); i++) {
        int j = XDino_RandomInt32(0, posCandidate.size() - 1);
        DinoVec2 temp = posCandidate[i];
        posCandidate[i] = posCandidate[j];
        posCandidate[j] = temp;
    }

    for (int i = 0; i < 10; i++) {
        DrawFlower(out, idxSeason, 0, posCandidate[i]);
    }
    for (int i = 10; i < 20; i++) {
        DrawFlower(out, idxSeason, 1, posCandidate[i]);
    }
    for (int i = 20; i < 30; i++) {
        DrawFlower(out, idxSeason, 2, posCandidate[i]);
    }
}

uint64_t DinoTerrain::CreateEgdes(int idxSeason, int idxAnim, DinoVec2 topLeft)
{
    std::vector<DinoVertex> vs;
    for (int ty = 0; ty < TILE_COUNT.y; ty++) {
        for (int tx = 0; tx < TILE_COUNT.x; tx++) {
            if (tx != 0 && tx != TILE_COUNT.x - 1 &&
                ty != 0 && ty != TILE_COUNT.y - 1) {
                continue;
            }
            CreateEdgeTiles(vs, idxSeason, topLeft, idxAnim, tx, ty);
        }
    }
    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Edges");
}

void DinoTerrain::CreateEdgeTiles(std::vector<DinoVertex>& out, int indexSeason, DinoVec2 topLeft, int idxAnim, int tx,
                                  int ty)
{
    int i = out.size();
    out.resize(i + 6);

    float x = topLeft.x + tx * 16.f;
    float y = topLeft.y + ty * 16.f;

    bool left = tx == 0;
    bool right = tx == TILE_COUNT.x - 1;
    bool top = ty == 0;
    bool bottom = ty == TILE_COUNT.y - 1;

    int u = left ? 0 : right ? 32 : 16;
    int v = top ? 16 : bottom ? 48 : 32;

    uint16_t uBase = indexSeason * 80;
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


void DinoTerrain::DrawFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos)
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