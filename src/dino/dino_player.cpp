#include "dino_player.h"

#include "dino_draw_utils.h"

#include <iostream>

constexpr float PLAYER_SPEED = 260;

DinoPlayer::DinoPlayer()
{
    gamepadIdx = {};
    pos = {};

    bMirror = false;
    bIdle = true;
    bWalking = false;
    bRunning = false;
}

void DinoPlayer::Init(DinoGamepadIdx gamepadIdx, DinoVec2 pos)
{
    this->gamepadIdx = gamepadIdx;
    this->pos = pos;
}

DinoPlayer::~DinoPlayer()
{
    // noop
}

void DinoPlayer::Update(float deltaTime)
{
    bIdle = true;
    bWalking = false;
    bRunning = false;

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
    if (!bSuccess)
        return;

    pos.x += gamepad.stick_left_x * PLAYER_SPEED * (gamepad.btn_right ? 1.5f : 1.f) * deltaTime;
    pos.y += gamepad.stick_left_y * PLAYER_SPEED * (gamepad.btn_right ? 1.5f : 1.f) * deltaTime;

    if (gamepad.stick_left_x != 0)
        bMirror = gamepad.stick_left_x < 0;

    bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!bIdle) {
        if (gamepad.btn_right) {
            bRunning = true;
        }
        else {
            bWalking = true;
        }
    }
}

void DinoPlayer::Draw(double timeSinceStart)
{
    int u = 0;
    if (bIdle) {
        u = 0;
        u += int(timeSinceStart / 0.125f) % 4 * 24;
    }
    if (bWalking) {
        u = 96;
        u += int(timeSinceStart / 0.125f) % 4 * 24;
    }
    if (bRunning) {
        u = 432;
        u += int(timeSinceStart / 0.0625f) % 6 * 24;
    }

    int v = static_cast<int>(gamepadIdx) * 24;

    DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite("dinosaurs.png", 24, 24, u, v, bMirror);
    drawCall.translation = pos;
    drawCall.scale = 1.5;
    XDino_Draw(drawCall);
}

bool DinoPlayer::DinoPlayerCompare(const DinoPlayer& a, const DinoPlayer& b)
{
    return a.pos.y < b.pos.y;
}