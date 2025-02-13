#include "dino_animals.h"

#include "dino_player.h"
#include "dino/xdino.h"


void DinoAnimals::UpdatePlayer(float deltaTime)
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

void DinoAnimals::DrawPlayer(double time)
{
    DinoDrawCall drawcall;
    drawcall.textureName = "animals.png";

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {32, 0};
    DinoVec2 posC = {0, 32};
    DinoVec2 posD = {32, 32};

    if (this->g_bMiror) {
        drawcall.vertices.emplace_back(posA, 32 , 0 + 32 * indexG);
        drawcall.vertices.emplace_back(posB, 0 , 0 + 32 * indexG);
        drawcall.vertices.emplace_back(posC, 32 , 32 + 32 * indexG);
        drawcall.vertices.emplace_back(posB, 0 , 0 + 32 * indexG);
        drawcall.vertices.emplace_back(posC, 32 , 32 + 32 * indexG);
        drawcall.vertices.emplace_back(posD, 0 , 32 + 32 * indexG);
    }
    else {
        drawcall.vertices.emplace_back(posA, 0 , 0 + 32 * indexG);
        drawcall.vertices.emplace_back(posB, 32 , 0 + 32 * indexG);
        drawcall.vertices.emplace_back(posC, 0 , 32 + 32 * indexG);
        drawcall.vertices.emplace_back(posB, 32 , 0 + 32 * indexG);
        drawcall.vertices.emplace_back(posC, 0 , 32 + 32 * indexG);
        drawcall.vertices.emplace_back(posD, 32 , 32 + 32 * indexG);
    }
    drawcall.translation = this->playerPos;
    drawcall.scale = 1;
    XDino_Draw(drawcall);
}

void DinoAnimals::Init()
{
    playerPos = XDino_RandomUnitVec2();
    playerPos = DinoVec2 {0,200};
    indexG = XDino_RandomUint32(0, 7);
}