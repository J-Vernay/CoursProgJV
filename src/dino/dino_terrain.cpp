#include <dino/dino_terrain.h>
#include <dino/dino_draw_utils.h>

#include <random>

void DinoTerrain::Init(int32_t tileCountX, int32_t tileCountY)
{
    DinoVec2 rdrSize = XDino_GetRenderSize();

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

    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16, 16}, 16, 0, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16.f * (tileCountX - 1), 16}, 32, 0, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16, 16.f * (tileCountY - 1)}, 16, 16, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16.f * (tileCountX - 1), 16}, 32, 0, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16, 16.f * (tileCountY - 1)}, 16, 16, DinoColor_WHITE);
    m_drawCallTerrain.vertices.emplace_back(DinoVec2{16.f * (tileCountX - 1), 16.f * (tileCountY - 1)},
                                            32,
                                            16,
                                            DinoColor_WHITE);

    // Fleurs

    // On ne génère pas de fleurs par-dessus les bordures du terrain.
    for (int32_t y = 1; y < tileCountY - 1; ++y) {
        for (int32_t x = 1; x < tileCountX - 1; ++x) {
            // 3 chances sur 20 de générer une fleur.
            int32_t flowerKind = XDino_RandomInt32(0, 9);
            if (flowerKind >= 3)
                continue;
            Dino_AddDraw_Rect(m_drawCallTerrain, {16.f * x, 16.f * y}, {16, 16}, {32 + 16.f * flowerKind, 0});
        }
    }
    // Grille centrée
    m_drawCallTerrain.translation.x = (rdrSize.x - (tileCountX * 16)) / 2;
    m_drawCallTerrain.translation.y = (rdrSize.y - (tileCountY * 16)) / 2;
}

void DinoTerrain::Draw(double timeSinceStart, float deltaTime)
{
    XDino_Draw(m_drawCallOcean);
    XDino_Draw(m_drawCallTerrain);
}