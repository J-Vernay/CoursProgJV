#include "DinoSpriteHandler.h"

void DinoSpriteHandler::ReadSprite(std::string textureName, DinoVec2 sPos, DinoVec2 ePos, DinoVec2 uv,
                                   DinoVec2 translate, float scale)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "terrain.png";
    drawCall.vertices.reserve(6);

    DinoVec2 posA = {sPos.x, sPos.y};
    DinoVec2 posB = {ePos.x, sPos.y};
    DinoVec2 posC = {sPos.x, ePos.y};
    DinoVec2 posD = {ePos.x, ePos.y};

    drawCall.vertices.emplace_back(posA, 0, 0);
    drawCall.vertices.emplace_back(posB, 0, 0);
    drawCall.vertices.emplace_back(posC, 0, 16);
    drawCall.vertices.emplace_back(posB, 16, 0);
    drawCall.vertices.emplace_back(posC, 0, 16);
    drawCall.vertices.emplace_back(posD, 16, 16);

    drawCall.translation = {0, 0};
    XDino_Draw(drawCall);
}