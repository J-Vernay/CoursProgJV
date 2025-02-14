#include "dino_player.h"
#include "dino/xdino.h"


void DinoPlayer::UpdatePlayer(float deltaTime)
{
    float speed;
    bool bMiror = this->g_bMiror;
    // Gestion des entrées et mise à jour de la logique de jeu.

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
        return;

    if (gamepad.btn_right) {
        speed = baseSpeed * 2;
        this->isRunning = true;
    }
    else {
        speed = baseSpeed;
        this->isRunning = false;
    }
    this->playerPos.x += gamepad.stick_left_x * speed * deltaTime;
    this->playerPos.y += gamepad.stick_left_y * speed * deltaTime;
    this->g_bMiror = bMiror;
}

void DinoPlayer::DrawPlayer(double time)
{
    DinoDrawCall drawcall;
    drawcall.textureName = "dinosaurs.png";

    DinoVec2 posA = {-24, -24};
    DinoVec2 posB = {0, -24};
    DinoVec2 posC = {-24, 0};
    DinoVec2 posD = {0, 0};

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
        drawcall.vertices.emplace_back(posA, 24 + u, 0 + 24 * indexG);
        drawcall.vertices.emplace_back(posB, 0 + u, 0 + 24 * indexG);
        drawcall.vertices.emplace_back(posC, 24 + u, 24 + 24 * indexG);
        drawcall.vertices.emplace_back(posB, 0 + u, 0 + 24 * indexG);
        drawcall.vertices.emplace_back(posC, 24 + u, 24 + 24 * indexG);
        drawcall.vertices.emplace_back(posD, 0 + u, 24 + 24 * indexG);
    }
    else {
        drawcall.vertices.emplace_back(posA, 0 + u, 0 + 24 * indexG);
        drawcall.vertices.emplace_back(posB, 24 + u, 0 + 24 * indexG);
        drawcall.vertices.emplace_back(posC, 0 + u, 24 + 24 * indexG);
        drawcall.vertices.emplace_back(posB, 24 + u, 0 + 24 * indexG);
        drawcall.vertices.emplace_back(posC, 0 + u, 24 + 24 * indexG);
        drawcall.vertices.emplace_back(posD, 24 + u, 24 + 24 * indexG);
    }
    drawcall.translation = this->playerPos;
    drawcall.scale = 1;
    XDino_Draw(drawcall);
}

void DinoPlayer::Init(DinoVec2 pos, DinoGamepadIdx idx, int indexGraph = 0)
{
    playerPos = pos;
    gamepadIdx = idx;
    indexG = indexGraph;
}