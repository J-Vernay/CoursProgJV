#include "dino_player.h"
#include "dino/xdino.h"


void DinoPlayer::UpdatePlayer(float deltaTime)
{
    float speed;
    bool bMiror = this->g_bMiror;
    // Gestion des entrées et mise à jour de la logique de jeu.

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (gamepad.stick_left_x < 0) {
            bMiror = true;
            this->isIdle = false;
            this->isWalking = true;
        }
        else if (gamepad.stick_left_x > 0) {
            bMiror = false;
            this->isIdle = false;
            this->isWalking = true;
        }
        else if (gamepad.stick_left_y != 0) {
            this->isIdle = false;
            this->isWalking = true;
        }
        else {
            this->isIdle = true;
            this->isWalking = false;
        }
        if (!bSuccess)
            continue;

        if (gamepad.btn_right) {
            speed = CIRCLE_SPEED * 2;
            this->isRunning = true;
        }
        else {
            speed = CIRCLE_SPEED;
            this->isRunning = false;
        }

        this->playerPos.x += gamepad.stick_left_x * speed * deltaTime;
        this->playerPos.y += gamepad.stick_left_y * speed * deltaTime;
        this->g_bMiror = bMiror;
    }
}

void DinoPlayer::DrawPlayer(float time)
{
    DinoDrawCall drawcall;
    drawcall.textureName = "dinosaurs.png";

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};

    int u = 0;
    if (this->isIdle) {
        int idxFrame = int(time * 8) % 4;
        u = 0 + 24 * idxFrame;
    }
    if (this->isWalking) {
        int idxFrame = int(time * 8) % 6;
        u = 96 + 24 * idxFrame;
    }
    if (this->isRunning) {
        int idxFrame = int(time * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    if (this->g_bMiror) {
        drawcall.vertices.emplace_back(posA, 24 + u, 0);
        drawcall.vertices.emplace_back(posB, 0 + u, 0);
        drawcall.vertices.emplace_back(posC, 24 + u, 24);
        drawcall.vertices.emplace_back(posB, 0 + u, 0);
        drawcall.vertices.emplace_back(posC, 24 + u, 24);
        drawcall.vertices.emplace_back(posD, 0 + u, 24);
    }
    else {
        drawcall.vertices.emplace_back(posA, 0 + u, 0);
        drawcall.vertices.emplace_back(posB, 24 + u, 0);
        drawcall.vertices.emplace_back(posC, 0 + u, 24);
        drawcall.vertices.emplace_back(posB, 24 + u, 0);
        drawcall.vertices.emplace_back(posC, 0 + u, 24);
        drawcall.vertices.emplace_back(posD, 24 + u, 24);
    }
    drawcall.translation = this->playerPos;
    drawcall.scale = 2;
    XDino_Draw(drawcall);
}