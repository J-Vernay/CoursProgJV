#include "dino_terrain.h"
#include <vector>

void DinoTerrain::Init(int tilesX, int tilesY, int tileSize, DinoVec2 origin)
{
    TILE_SIZE = tileSize;
    TILES_X = tilesX;
    TILES_Y = tilesY;
    terrainOrigin = origin;

    texID_terrain = XDino_CreateGpuTexture("terrain.png");

    season = XDino_RandomInt32(0, 3);
    biomeOffset = season * 80;

    std::vector<DinoVertex> vs;

    for (int y = 0; y < TILES_Y; ++y) {
        for (int x = 0; x < TILES_X; ++x) {

            int u = 0;
            int v = 0;

            if (x == 0 && y == 0) {
                u = 0;
                v = 16;
            }
            else if (x == TILES_X - 1 && y == 0) {
                u = 32;
                v = 16;
            }
            else if (x == 0 && y == TILES_Y - 1) {
                u = 0;
                v = 48;
            }
            else if (x == TILES_X - 1 && y == TILES_Y - 1) {
                u = 32;
                v = 48;
            }
            else if (y == 0) {
                u = 16;
                v = 16;
            }
            else if (y == TILES_Y - 1) {
                u = 16;
                v = 48;
            }
            else if (x == 0) {
                u = 0;
                v = 32;
            }
            else if (x == TILES_X - 1) {
                u = 32;
                v = 32;
            }
            else {
                u = 16;
                v = 0;
            }

            u += biomeOffset;

            float px = terrainOrigin.x + x * TILE_SIZE;
            float py = terrainOrigin.y + y * TILE_SIZE;

            int i = vs.size();
            vs.resize(i + 6);

            vs[i + 0].pos = {px, py};
            vs[i + 1].pos = {px + TILE_SIZE, py};
            vs[i + 2].pos = {px, py + TILE_SIZE};

            vs[i + 3].pos = {px + TILE_SIZE, py};
            vs[i + 4].pos = {px, py + TILE_SIZE};
            vs[i + 5].pos = {px + TILE_SIZE, py + TILE_SIZE};

            int u1 = u + TILE_SIZE;
            int v1 = v + TILE_SIZE;

            vs[i + 0].u = u;
            vs[i + 0].v = v;

            vs[i + 1].u = u1;
            vs[i + 1].v = v;

            vs[i + 2].u = u;
            vs[i + 2].v = v1;

            vs[i + 3].u = u1;
            vs[i + 3].v = v;

            vs[i + 4].u = u;
            vs[i + 4].v = v1;

            vs[i + 5].u = u1;
            vs[i + 5].v = v1;

            for (int k = 0; k < 6; ++k)
                vs[i + k].color = DinoColor_WHITE;
        }
    }

    terrainVbuf = XDino_CreateVertexBuffer(vs.data(), vs.size(), "terrain");

    // fleurs
    flowers.clear();

    struct Cell {
        int x, y;
    };
    std::vector<Cell> cells;

    for (int y = 1; y < TILES_Y - 1; ++y)
        for (int x = 1; x < TILES_X - 1; ++x)
            cells.push_back({x, y});

    for (int i = cells.size() - 1; i > 0; --i) {
        int j = XDino_RandomInt32(0, i);
        std::swap(cells[i], cells[j]);
    }

    int cellIndex = 0;

    for (int type = 0; type < 3; ++type)
        for (int i = 0; i < 10; ++i) {

            Cell c = cells[cellIndex++];

            Flower f;
            f.pos.x = terrainOrigin.x + c.x * TILE_SIZE + TILE_SIZE / 2;
            f.pos.y = terrainOrigin.y + c.y * TILE_SIZE + TILE_SIZE / 2;
            f.type = type;

            flowers.push_back(f);
        }
}

void DinoTerrain::Draw(double timeSinceStart)
{
    DrawOcean();

    XDino_Draw(terrainVbuf, texID_terrain);

    DrawFlowers();
}

void DinoTerrain::DrawOcean() const
{
    std::vector<DinoVertex> vs(6);

    vs[0].pos = {0, 0};
    vs[1].pos = {480, 0};
    vs[2].pos = {0, 360};
    vs[3].pos = {480, 0};
    vs[4].pos = {0, 360};
    vs[5].pos = {480, 360};

    for (auto& v : vs) {
        v.color = DinoColor_WHITE;
        v.u = biomeOffset;
        v.v = 0;
    }

    uint64_t vbuf = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ocean");
    XDino_Draw(vbuf, texID_terrain);
    XDino_DestroyVertexBuffer(vbuf);
}

void DinoTerrain::DrawFlowers() const
{
    for (const Flower& f : flowers) {

        std::vector<DinoVertex> vs(6);

        vs[0].pos = {-8, -8};
        vs[1].pos = {8, -8};
        vs[2].pos = {-8, 8};
        vs[3].pos = {8, -8};
        vs[4].pos = {-8, 8};
        vs[5].pos = {8, 8};

        for (auto& v : vs)
            v.color = DinoColor_WHITE;

        int flowerU0 = 32 + f.type * 16 + biomeOffset;
        int flowerU1 = flowerU0 + 16;

        vs[0].u = flowerU0;
        vs[0].v = 0;
        vs[1].u = flowerU1;
        vs[1].v = 0;
        vs[2].u = flowerU0;
        vs[2].v = 16;

        vs[3].u = flowerU1;
        vs[3].v = 0;
        vs[4].u = flowerU0;
        vs[4].v = 16;
        vs[5].u = flowerU1;
        vs[5].v = 16;

        uint64_t vbuf = XDino_CreateVertexBuffer(vs.data(), vs.size(), "flower");
        XDino_Draw(vbuf, texID_terrain, f.pos);
        XDino_DestroyVertexBuffer(vbuf);
    }
}

void DinoTerrain::Shut()
{
    XDino_DestroyVertexBuffer(terrainVbuf);
    XDino_DestroyGpuTexture(texID_terrain);
}

DinoVec2 DinoTerrain::GetRandomCellCenter() const
{
    int x = XDino_RandomInt32(1, TILES_X - 2);
    int y = XDino_RandomInt32(1, TILES_Y - 2);

    return {
        terrainOrigin.x + x * TILE_SIZE + TILE_SIZE / 2,
        terrainOrigin.y + y * TILE_SIZE + TILE_SIZE / 2
    };
}