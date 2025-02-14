#include "DinoTerrain.h"

DinoTerrain::DinoTerrain()
{
}

void DinoTerrain::DrawWater(DinoVec2 renderSize)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "terrain.png";
    drawCall.vertices.reserve(6);

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {renderSize.x, 0};
    DinoVec2 posC = {0, renderSize.y};
    DinoVec2 posD = {renderSize.x, renderSize.y};

    drawCall.vertices.emplace_back(posA, 0, 0);
    drawCall.vertices.emplace_back(posB, 0, 0);
    drawCall.vertices.emplace_back(posC, 0, 16);
    drawCall.vertices.emplace_back(posB, 16, 0);
    drawCall.vertices.emplace_back(posC, 0, 16);
    drawCall.vertices.emplace_back(posD, 16, 16);

    drawCall.translation = {0, 0};
    XDino_Draw(drawCall);
}

void DinoTerrain::DrawLand(DinoVec2 renderSize, DinoVec2 landSize)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "terrain.png";
    drawCall.vertices.reserve(6);

    DinoVec2 pos;
    pos.x = (renderSize.x - landSize.x) * 0.5f;
    pos.y = (renderSize.y - landSize.y) * 0.5f;

    DinoVec2 posA = {pos.x, pos.y};
    DinoVec2 posB = {pos.x + landSize.x, pos.y};
    DinoVec2 posC = {pos.x, pos.y + landSize.y};
    DinoVec2 posD = {pos.x + landSize.x, pos.y + landSize.y};

    drawCall.vertices.emplace_back(posA, 16, 0);
    drawCall.vertices.emplace_back(posB, 32, 0);
    drawCall.vertices.emplace_back(posC, 16, 16);
    drawCall.vertices.emplace_back(posB, 32, 0);
    drawCall.vertices.emplace_back(posC, 16, 16);
    drawCall.vertices.emplace_back(posD, 32, 16);

    drawCall.translation = {0, 0};
    XDino_Draw(drawCall);
}

void DrawGoodLand(DinoVec2 renderSize, DinoVec2 landSize)
{
    DinoVec2 indexes = {landSize.x / 16, landSize.y / 16};

    for (int i = 0; i < indexes.x; i++) {
        for (int j = 0; j < indexes.y; j++) {
            
        }
    }
}