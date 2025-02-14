#include "dino_terrain.h"
#include <dino/xdino.h>

// Fonction pour dessiner le terrain.
void dino_terrain::DrawTerrain(DinoVec2 renderSize)
{
    DinoVec2 terrainSize = {256, 192};
    DinoDrawCall drawCall;
    drawCall.textureName = "terrain.png";

    DinoVec2 posA, posB, posC, posD;
    posA.x = (renderSize.x - terrainSize.x) / 2;
    posA.y = (renderSize.y - terrainSize.y) / 2;
    posB.x = posA.x + terrainSize.x;
    posB.y = posA.y;
    posC.x = posA.x;
    posC.y = posA.y + terrainSize.y;
    posD.x = posA.x + terrainSize.x;
    posD.y = posA.y + terrainSize.y;

    // Océan en fond
    drawCall.vertices.emplace_back(DinoVec2{0, 0}, 0, 0);
    drawCall.vertices.emplace_back(DinoVec2{renderSize.x, 0}, 16, 0);
    drawCall.vertices.emplace_back(DinoVec2{0, renderSize.y}, 0, 16);
    drawCall.vertices.emplace_back(DinoVec2{renderSize.x, 0}, 16, 0);
    drawCall.vertices.emplace_back(DinoVec2{0, renderSize.y}, 0, 16);
    drawCall.vertices.emplace_back(DinoVec2{renderSize.x, renderSize.y}, 16, 16);

    // Terrain au milieu
    drawCall.vertices.emplace_back(posA, 16, 0);
    drawCall.vertices.emplace_back(posB, 32, 0);
    drawCall.vertices.emplace_back(posC, 16, 16);
    drawCall.vertices.emplace_back(posB, 32, 0);
    drawCall.vertices.emplace_back(posC, 16, 16);
    drawCall.vertices.emplace_back(posD, 32, 16);

    XDino_Draw(drawCall);
}