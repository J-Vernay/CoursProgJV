#include "dino_player.h"

#include "x64-windows/xdino_win64_rdr.h"

void DinoPlayer::UpdatePlayer(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize)
{
    this->idle = false;
    this->walking = false;
    this->running = false;

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(DinoGamepadIdx_ALL[playerIndex], gamepad);
    if (!bSuccess)
        return;

    float speed = 300;
    if (gamepad.btn_right)
        speed *= 2;

    float nextPosX = pos.x + gamepad.stick_left_x * speed * deltaTime;
    float nextPosY = pos.y + gamepad.stick_left_y * speed * deltaTime;

    if (nextPosX > windowSize.x - (terrainSize.x / 2))
        nextPosX = windowSize.x - (terrainSize.x / 2);
    if (nextPosX < windowSize.x - (windowSize.x - terrainSize.x / 2))
        nextPosX = windowSize.x - (windowSize.x - terrainSize.x / 2);
    if (nextPosY > windowSize.y - (terrainSize.y / 2))
        nextPosY = windowSize.y - (terrainSize.y / 2);
    if (nextPosY < windowSize.y - (windowSize.y - terrainSize.y / 2))
        nextPosY = windowSize.y - (windowSize.y - terrainSize.y / 2);

    pos.x = nextPosX;
    pos.y = nextPosY;

    if (gamepad.stick_left_x != 0)
        this->goingRight = gamepad.stick_left_x > 0;

    this->idle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!this->idle) {
        if (gamepad.btn_right) {
            this->running = true;
        }
        else {
            this->walking = true;
        }
    }
}

void DinoPlayer::DrawPlayer(double timeSinceStart)
{
    DinoDrawCall drawCall;
    float radiusX = 12;
    float radiusY = 22;
    drawCall.textureName = "dinosaurs.png";
    drawCall.vertices.reserve(6);
    if (!this->goingRight)
        radiusX = -radiusX;

    DinoVec2 posA = {-radiusX, -radiusY};
    DinoVec2 posB = {radiusX, -radiusY};
    DinoVec2 posC = {-radiusX, 2};
    DinoVec2 posD = {radiusX, 2};
    DinoColor color = DinoColor_WHITE;

    float secondsPerSprite;
    if (this->running)
        secondsPerSprite = 0.0625;
    else {

        secondsPerSprite = 0.125;
    }

    int animationIndex = int(timeSinceStart / secondsPerSprite) % 4 * 24;

    if (this->walking)
        animationIndex += 96;
    if (this->running)
        animationIndex += 432;

    int v = playerIndex * 24;

    drawCall.vertices.emplace_back(posA, animationIndex, v + 0, color);
    drawCall.vertices.emplace_back(posB, animationIndex + 24, v + 0, color);
    drawCall.vertices.emplace_back(posC, animationIndex, v + 24, color);
    drawCall.vertices.emplace_back(posB, animationIndex + 24, v + 0, color);
    drawCall.vertices.emplace_back(posC, animationIndex, v + 24, color);
    drawCall.vertices.emplace_back(posD, animationIndex + 24, v + 24, color);
    drawCall.translation = this->pos;
    XDino_Draw(drawCall);
}

void DinoPlayer::Init(int idx, DinoVec2 posInit)
{
    pos = posInit;
    playerIndex = idx;
}

bool DinoPlayer::IsAbove(DinoPlayer& player)
{
    if (player.pos.y >= pos.y) {
        return false;
    }
    else {
        return true;
    }
}