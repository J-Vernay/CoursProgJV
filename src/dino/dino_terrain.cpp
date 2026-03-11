#include "dino_terrain.h"
#include <random>

void DinoTerrain::Init(int tilesX, int tilesY, int tileSize, DinoVec2 origin)
{
    TILE_SIZE = tileSize;
    TILES_X = tilesX;
    TILES_Y = tilesY;
    terrainOrigin = origin;

    texID_terrain = XDino_CreateGpuTexture("terrain.png");
    season = XDino_RandomInt32(0, 3);

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
    DrawGround();
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
        v.u = 0 + biomeOffset;
        v.v = 0;
    }

    uint64_t vbuf = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ocean");
    XDino_Draw(vbuf, texID_terrain);
    XDino_DestroyVertexBuffer(vbuf);
}

void DinoTerrain::DrawGround() const
{
    for (int y = 0; y < TILES_Y; ++y) {
        for (int x = 0; x < TILES_X; ++x) {

            int u = 0;
            int v = 0;

            // Coins
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

            // Bords
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

            // Intérieur
            else {
                u = 16;
                v = 0;
            }

            u += biomeOffset;

            float px = terrainOrigin.x + x * TILE_SIZE;
            float py = terrainOrigin.y + y * TILE_SIZE;

            std::vector<DinoVertex> vs(6);

            vs[0].pos = {0, 0};
            vs[1].pos.x = TILE_SIZE;
            vs[1].pos.y = 0;
            vs[2].pos.x = 0;
            vs[2].pos.y = TILE_SIZE;
            vs[3].pos.x = TILE_SIZE;
            vs[3].pos.y = 0;
            vs[4].pos.x = 0;
            vs[4].pos.y = TILE_SIZE;
            vs[5].pos.x = TILE_SIZE;
            vs[5].pos.y = TILE_SIZE;

            for (auto& vtx : vs)
                vtx.color = DinoColor_WHITE;

            int u1 = u + TILE_SIZE;
            int v1 = v + TILE_SIZE;

            vs[0].u = u;
            vs[0].v = v;

            vs[1].u = u1;
            vs[1].v = v;

            vs[2].u = u;
            vs[2].v = v1;

            vs[3].u = u1;
            vs[3].v = v;

            vs[4].u = u;
            vs[4].v = v1;

            vs[5].u = u1;
            vs[5].v = v1;

            uint64_t vbuf = XDino_CreateVertexBuffer(vs.data(), vs.size(), "tile");
            XDino_Draw(vbuf, texID_terrain, {px, py});
            XDino_DestroyVertexBuffer(vbuf);
        }
    }
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

        // Fleur 1 : 32, Fleur2 : 48, Fleur3 : 64 + offset de saison
        int flowerU0 = 32 + f.type * 16 + biomeOffset;
        int flowerU1 = flowerU0 + 16;

        vs[0].u = flowerU1;
        vs[0].v = 0;
        vs[1].u = flowerU0;
        vs[1].v = 0;
        vs[2].u = flowerU1;
        vs[2].v = 16;
        vs[3].u = flowerU0;
        vs[3].v = 0;
        vs[4].u = flowerU1;
        vs[4].v = 16;
        vs[5].u = flowerU0;
        vs[5].v = 16;

        uint64_t vbuf = XDino_CreateVertexBuffer(vs.data(), vs.size(), "flower");
        XDino_Draw(vbuf, texID_terrain, f.pos);
        XDino_DestroyVertexBuffer(vbuf);
    }
}

void DinoTerrain::Shut()
{
    XDino_DestroyGpuTexture(texID_terrain);
}

DinoVec2 DinoTerrain::GetRandomCellCenter() const
{
    if (TILES_X < 2 || TILES_Y < 2)
        return terrainOrigin;

    int x = XDino_RandomInt32(1, TILES_X - 2);
    int y = XDino_RandomInt32(1, TILES_Y - 2);

    DinoVec2 center;
    center.x = terrainOrigin.x + x * TILE_SIZE + TILE_SIZE / 2;
    center.y = terrainOrigin.y + y * TILE_SIZE + TILE_SIZE / 2;

    return center;
}