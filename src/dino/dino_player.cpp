#include "dino_player.h"

void dino_player::InitDino(DinoVec2 position, int index)
{
    pos = position;
    idxPlayer = index;
}

void dino_player::UpdatePlayer(float deltaTime)
{
    bIdle = false;
    bRunning = false;
    bWalking = false;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        float speed = CIRCLE_SPEED;
        if (gamepad.btn_right) {
            speed *= 2;
        }

        pos.x += gamepad.stick_left_x * speed * deltaTime;
        pos.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x != 0) {
            bMirror = gamepad.stick_left_x < 0;
        }

        bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
        if (!bIdle) {
            if (gamepad.btn_right)
                bRunning = true;
            else
                bWalking = true;
        }
    }

}

void dino_player::DrawDino(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};

    int u = 0;
    if (bIdle) {
        int idxFrame = static_cast<int>(timeSinceStart * 8) % 4;
        u = 0 + 24 * idxFrame;
    }

    if (bWalking) {
        int idxFrame = static_cast<int>(timeSinceStart * 8) % 6;
        u = 96 + 24 * idxFrame;
    }

    if (bRunning) {
        int idxFrame = static_cast<int>(timeSinceStart * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    int v = idxPlayer * 24;

    if (bMirror) {
        drawCall.vertices.emplace_back(posA, u + 24, v + 0);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posD, u + 0, v + 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, v + 0);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posD, u + 24, v + 24);
    }

    drawCall.scale = 3;
    drawCall.translation = pos;
    XDino_Draw(drawCall);
}