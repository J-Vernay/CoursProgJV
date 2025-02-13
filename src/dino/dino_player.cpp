#include "dino_player.h"

void DinoPlayer::UpdatePlayer(float deltaTime)
{
    this->idle = false;
    this->walking = false;
    this->running = false;

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(DinoGamepadIdx_ALL[playerIndex], gamepad);
    if (!bSuccess)
        return;

    /*if (gamepad.btn_down && !gamepad.btn_up)
        scale /= 1.01;
    if (gamepad.btn_up && !gamepad.btn_down)
        scale *= 1.01;
    if (gamepad.btn_left && !gamepad.btn_right)
        rotation += 90.0 * deltaTime;
    if (gamepad.btn_right && !gamepad.btn_left)
        rotation -= 90.0 * deltaTime;*/

    float speed = 300;
    if (gamepad.btn_right)
        speed *= 2;

    this->pos.x += gamepad.stick_left_x * speed * deltaTime;
    this->pos.y += gamepad.stick_left_y * speed * deltaTime;
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
    float radiusX = 24;
    float radiusY = 24;
    drawCall.textureName = "dinosaurs.png";
    drawCall.vertices.reserve(6);
    if (!this->goingRight)
        radiusX = -radiusX;

    DinoVec2 posA = {-radiusX, -radiusY};
    DinoVec2 posB = {radiusX, -radiusY};
    DinoVec2 posC = {-radiusX, radiusY};
    DinoVec2 posD = {radiusX, radiusY};
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