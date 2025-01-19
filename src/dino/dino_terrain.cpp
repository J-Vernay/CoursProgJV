#include <dino/dino_terrain.h>
#include <dino/dino_draw_utils.h>

#include <random>

void DinoTerrain::Init(int32_t tileCountX, int32_t tileCountY)
{
    DinoVec2 rdrSize = XDino_GetRenderSize();

    DinoVec2 terrainSize, terrainOffset;
    terrainSize.x = tileCountX * 16.f;
    terrainSize.y = tileCountY * 16.f;
    terrainOffset.x = 0.5f * (rdrSize.x - terrainSize.x);
    terrainOffset.y = 0.5f * (rdrSize.y - terrainSize.y);

    m_spawnOffset = {terrainOffset.x + 16.f, terrainOffset.y + 16.f};
    m_spawnSize = {terrainSize.x - 32.f, terrainSize.y - 32.f};

    // Océan, draw call séparé car il doit représenter tout l'écran, alors que le m_drawCallTerrain peut être translaté.
    m_drawCallOcean.textureName = "terrain.png";
    m_drawCallOcean.vertices.emplace_back(DinoVec2{0, 0}, 0, 0, DinoColor_WHITE);
    m_drawCallOcean.vertices.emplace_back(DinoVec2{rdrSize.x, 0}, 16, 0, DinoColor_WHITE);
    m_drawCallOcean.vertices.emplace_back(DinoVec2{0, rdrSize.y}, 0, 16, DinoColor_WHITE);
    m_drawCallOcean.vertices.emplace_back(DinoVec2{rdrSize.x, 0}, 16, 0, DinoColor_WHITE);
    m_drawCallOcean.vertices.emplace_back(DinoVec2{0, rdrSize.y}, 0, 16, DinoColor_WHITE);
    m_drawCallOcean.vertices.emplace_back(rdrSize, 16, 16, DinoColor_WHITE);
    XDino_Draw(m_drawCallOcean);

    m_drawCallTerrain.textureName = "terrain.png";

    // Coins

    Dino_AddDraw_Rect(m_drawCallTerrain, {0, 0}, {16, 16}, {0, 16});
    Dino_AddDraw_Rect(m_drawCallTerrain, {16.f * (tileCountX - 1), 0}, {16, 16}, {32, 16});
    Dino_AddDraw_Rect(m_drawCallTerrain, {0, 16.f * (tileCountY - 1)}, {16, 16}, {0, 48});
    Dino_AddDraw_Rect(m_drawCallTerrain, {16.f * (tileCountX - 1), 16.f * (tileCountY - 1)}, {16, 16}, {32, 48});

    // Frontières

    for (int32_t i = 1; i < tileCountX - 1; ++i) {
        Dino_AddDraw_Rect(m_drawCallTerrain, {16.f * i, 0}, {16, 16}, {16, 16});
        Dino_AddDraw_Rect(m_drawCallTerrain, {16.f * i, 16.f * (tileCountY - 1)}, {16, 16}, {16, 48});
    }
    for (int32_t i = 1; i < tileCountY - 1; ++i) {
        Dino_AddDraw_Rect(m_drawCallTerrain, {0, 16.f * i}, {16, 16}, {0, 32});
        Dino_AddDraw_Rect(m_drawCallTerrain, {16.f * (tileCountX - 1), 16.f * i}, {16, 16}, {32, 32});
    }

    // Milieu

    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16, 16}, 16, 32, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16.f * (tileCountX - 1), 16}, 32, 32, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16, 16.f * (tileCountY - 1)}, 16, 48, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16.f * (tileCountX - 1), 16}, 32, 32, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16, 16.f * (tileCountY - 1)}, 16, 48, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16.f * (tileCountX - 1), 16.f * (tileCountY - 1)},
                                            32,
                                            48,
                                            DinoColor_WHITE);

    // Grille centrée
    m_drawCallTerrain.translation = terrainOffset;

    // Fleurs, draw call séparé pour pouvoir gérer l'animation du terrain différemment des fleurs.

    // On ne génère pas de fleurs par-dessus les bordures du terrain.
    m_drawCallFlowers.textureName = "terrain.png";
    for (int32_t y = 1; y < tileCountY - 1; ++y) {
        for (int32_t x = 1; x < tileCountX - 1; ++x) {
            // 3 chances sur 30 de générer une fleur.
            int32_t flowerKind = XDino_RandomInt32(0, 29);
            if (flowerKind >= 3)
                continue;
            Dino_AddDraw_Rect(m_drawCallFlowers, {16.f * x, 16.f * y}, {16, 16}, {32 + 16.f * flowerKind, 0});
        }
    }
    // On garde les mêmes coordonnées
    m_drawCallFlowers.translation = terrainOffset;
}

void DinoTerrain::Draw(double timeSinceStart, float deltaTime) const
{
    XDino_Draw(m_drawCallOcean);

    DinoDrawCall animTerrain = m_drawCallTerrain; // Copie pour changer les UV pour prendre en compte l'animation

    constexpr uint16_t OFFSETS_V[6] = {0, 48, 96, 144, 96, 48};
    int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 6;
    uint16_t offsetV = OFFSETS_V[idxFrame];

    for (DinoVertex& vertex : animTerrain.vertices) {
        vertex.v += offsetV;
    }
    XDino_Draw(animTerrain);

    XDino_Draw(m_drawCallFlowers);
}

DinoVec2 DinoTerrain::GenerateRandomSpawn() const
{
    DinoVec2 pos;
    pos.x = m_spawnOffset.x + XDino_RandomFloat(0, m_spawnSize.x);
    pos.y = m_spawnOffset.y + XDino_RandomFloat(0, m_spawnSize.y);
    return pos;
}