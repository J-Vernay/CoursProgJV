#include "dino_animal.h"

void DinoAnimal::UpdateAnimal(float deltaTime)
{
    if (speed != 0) {
        pos.x += dir.x * speed * deltaTime;
        pos.y += dir.y * speed * deltaTime;
        goingRight = dir.x > 0;
    }

}

void DinoAnimal::DrawAnimal(double timeSinceStart)
{
    DinoDrawCall drawCall;
    float radiusX = 32;
    float radiusY = 32;
    drawCall.textureName = "animal.png";
    drawCall.vertices.reserve(6);
    if (!this->goingRight)
        radiusX = -radiusX;

    DinoVec2 posA = {-radiusX, -radiusY};
    DinoVec2 posB = {radiusX, -radiusY};
    DinoVec2 posC = {-radiusX, radiusY};
    DinoVec2 posD = {radiusX, radiusY};
    DinoColor color = DinoColor_WHITE;

    float secondsPerSprite = 0.125;

    int animationIndex = int(timeSinceStart / secondsPerSprite) % 4 * 128;

    int v = animalType * 32;

    drawCall.vertices.emplace_back(posA, animationIndex, v + 0, color);
    drawCall.vertices.emplace_back(posB, animationIndex + 32, v + 0, color);
    drawCall.vertices.emplace_back(posC, animationIndex, v + 32, color);
    drawCall.vertices.emplace_back(posB, animationIndex + 32, v + 0, color);
    drawCall.vertices.emplace_back(posC, animationIndex, v + 32, color);
    drawCall.vertices.emplace_back(posD, animationIndex + 32, v + 32, color);
    drawCall.translation = this->pos;
    XDino_Draw(drawCall);
}

void DinoAnimal::SpawnAnimal(int type)
{
    if (speed == 0) {
        pos = XDino_RandomUnitVec2();
        pos.x = XDino_RandomFloat(0, XDino_GetRenderSize().x);
        pos.y = XDino_RandomFloat(0, XDino_GetRenderSize().y);
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