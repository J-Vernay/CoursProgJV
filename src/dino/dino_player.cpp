#include "dino_player.h"
#include "dino/xdino.h"


void DinoPlayer::Update(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.

    this->isIdle = false;
    this->isWalking = false;
    this->isRunning = false;

    DinoGamepad gamepad;
    bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
    if (!bSuccess)
        gamepad = {}; // Laisser vide, on considère le joueur immobile

    float speed = baseSpeed;
    if (gamepad.btn_right)
        speed *= 2;

    this->playerPos.x += gamepad.stick_left_x * speed * deltaTime;
    this->playerPos.y += gamepad.stick_left_y * speed * deltaTime;

    if (gamepad.stick_left_x != 0)
        this->g_bMiror = gamepad.stick_left_x < 0;

    this->isIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!this->isIdle) {
        if (gamepad.btn_right)
            this->isRunning = true;
        else
            this->isWalking = true;
    }
}

void DinoPlayer::Draw(double time)
{
    DinoDrawCall drawcall;
    drawcall.textureName = "greyscale_Dino.png";

    DinoVec2 posA = {-12, -22};
    DinoVec2 posB = {12, -22};
    DinoVec2 posC = {-12, 2};
    DinoVec2 posD = {12, 2};

    DinoColor color = indexG == 0
                          ? DinoColor_RED
                          : indexG == 1
                          ? DinoColor_BLUE
                          : indexG == 2
                          ? DinoColor_YELLOW
                          : DinoColor_GREEN;

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
    drawcall.scale = 1;
    for (DinoVertex& vertice : drawcall.vertices) {
        vertice.color = color;

    }
    XDino_Draw(drawcall);
}

void DinoPlayer::Init(DinoVec2 pos, DinoGamepadIdx idx, int indexGraph = 0)
{
    playerPos = pos;
    gamepadIdx = idx;
    indexG = indexGraph;
}