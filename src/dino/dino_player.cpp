#include "dino_player.h"

#include <iostream>

constexpr float PLAYER_SPEED = 300;

DinoPlayer::DinoPlayer()
{
    gamepadIdx = {};
    pos = {};

    bMirror = false;
    bIdle = false;
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
    bIdle = false;
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
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png"; // Ici on change en dinosaurs pour avoir accès au sprite sheet. 
    drawCall.vertices.reserve(6);
    DinoVec2 posA = {-24, -24};
    DinoVec2 posB = {24, -24};
    DinoVec2 posC = {-24, 24};
    DinoVec2 posD = {24, 24};

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

    if (bMirror) {
        drawCall.vertices.emplace_back(posA, u + 24, v + 0);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posD, u + 0, v + 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, v + 0); // J'ai mis le premier dinosaure bleu
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posD, u + 24, v + 24);
    }
    drawCall.translation = pos;
    drawCall.scale = 2;
    XDino_Draw(drawCall);
}