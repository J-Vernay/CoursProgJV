#include "dino_animal.h"

void DinoAnimal::UpdateAnimal(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize)
{
    if (speed != 0) {
        float nextPosX = pos.x + (dir.x * speed * deltaTime);
        float nextPosY = pos.y + (dir.y * speed * deltaTime);

        if (nextPosX > windowSize.x - (terrainSize.x / 2)) {
            nextPosX = windowSize.x - (terrainSize.x / 2);
            dir = XDino_RandomUnitVec2();
        }
        if (nextPosX < windowSize.x - (windowSize.x - terrainSize.x / 2)) {
            nextPosX = windowSize.x - (windowSize.x - terrainSize.x / 2);
            dir = XDino_RandomUnitVec2();
        }
        if (nextPosY > windowSize.y - (terrainSize.y / 2)) {
            nextPosY = windowSize.y - (terrainSize.y / 2);
            dir = XDino_RandomUnitVec2();
        }
        if (nextPosY < windowSize.y - (windowSize.y - terrainSize.y / 2)) {
            nextPosY = windowSize.y - (windowSize.y - terrainSize.y / 2);
            dir = XDino_RandomUnitVec2();
        }

        pos.x = nextPosX;
        pos.y = nextPosY;
        goingRight = dir.x <= 0;
    }

}

void DinoAnimal::DrawAnimal(double timeSinceStart)
{
    DinoDrawCall drawCall;
    float radiusX = 12;
    float radiusY = 22;
    drawCall.textureName = "animals.png";
    drawCall.vertices.reserve(6);
    if (!this->goingRight)
        radiusX = -radiusX;

    DinoVec2 posA = {-radiusX, -radiusY};
    DinoVec2 posB = {radiusX, -radiusY};
    DinoVec2 posC = {-radiusX, 2};
    DinoVec2 posD = {radiusX, 2};
    DinoColor color = DinoColor_WHITE;

    float secondsPerSprite = 0.125;

    int animationIndex = int(timeSinceStart / secondsPerSprite) % 4 * 32 + (128 * animalType);

    int v = 0;

    drawCall.vertices.emplace_back(posA, animationIndex, v + 0, color);
    drawCall.vertices.emplace_back(posB, animationIndex + 32, v + 0, color);
    drawCall.vertices.emplace_back(posC, animationIndex, v + 32, color);
    drawCall.vertices.emplace_back(posB, animationIndex + 32, v + 0, color);
    drawCall.vertices.emplace_back(posC, animationIndex, v + 32, color);
    drawCall.vertices.emplace_back(posD, animationIndex + 32, v + 32, color);
    drawCall.translation = this->pos;
    XDino_Draw(drawCall);
}

void DinoAnimal::SpawnAnimal(int type, DinoVec2 renderSize, DinoVec2 terrainSize)
{
    if (speed == 0) {
        float maxX = renderSize.x - (terrainSize.x / 2);
        float minX = renderSize.x - (renderSize.x - terrainSize.x / 2);
        float maxY = renderSize.y - (terrainSize.y / 2);
        float minY = renderSize.y - (renderSize.y - terrainSize.y / 2);

        pos.x = XDino_RandomFloat(minX, maxX);
        pos.y = XDino_RandomFloat(minY, maxY);
        animalType = type;
        dir = XDino_RandomUnitVec2();
        speed = (float)XDino_RandomInt32(10, 50);
    }
}

bool DinoAnimal::IsAbove(DinoAnimal& player)
{
    if (player.pos.y >= pos.y) {
        return false;
    }
    else {
        return true;
    }
}