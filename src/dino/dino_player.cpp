#include "dino_player.h"
#include <dino/xdino.h>

// Constantes.
constexpr float speed = 300.f; // Nombre de pixels parcourus en une seconde.

void dino_player::Init(DinoVec2 posInit, int index, DinoGamepadIdx idxGamepad)
{
    playerPos = posInit;
    indexPlayer = index;
}

void dino_player::UpdatePlayer(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    isIdle = false;
    isWalking = false;
    isRunning = false;

    DinoGamepad gamepad{};

    bool bSuccess = XDino_GetGamepad(DinoGamepadIdx_ALL[indexPlayer], gamepad);
    if (!bSuccess)
        return;

    float playerSpeed = speed;

    if (gamepad.btn_right && !gamepad.btn_left) {
        playerSpeed *= 2.f;
    }

    playerPos.x += gamepad.stick_left_x * playerSpeed * deltaTime;
    playerPos.y += gamepad.stick_left_y * playerSpeed * deltaTime;

    if (gamepad.stick_left_x != 0) {
        isMirror = gamepad.stick_left_x < 0;
    }

    isIdle = gamepad.stick_left_x == 00 && gamepad.stick_left_y == 0;
    if (!isIdle) {
        if (gamepad.btn_right)
            isRunning = true;
        else {
            isWalking = true;
        }
    }
}

void dino_player::DrawPlayer(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png"; // Ici on accède au fichier dinosaurs.png, le sprite sheet des dinosaures.
    drawCall.vertices.reserve(6);

    DinoVec2 posA = {-12, -22};
    DinoVec2 posB = {12, -22};
    DinoVec2 posC = {-12, 2};
    DinoVec2 posD = {12, 2};
    DinoColor color = DinoColor_WHITE;

    int animationIndex = 0;

    if (isIdle) {
        int indexFrame = int(timeSinceStart * 8) % 4;
        animationIndex = 0 + 24 * indexFrame;
    }
    if (isWalking) {
        int indexFrame = int(timeSinceStart * 8) % 6;
        animationIndex = 96 + 24 * indexFrame;
    }
    if (isRunning) {
        int indexFrame = int(timeSinceStart * 16) % 6;
        animationIndex = 432 + 24 * indexFrame;
    }

    int v = indexPlayer * 24;

    if (isMirror) {
        drawCall.vertices.emplace_back(posA, animationIndex + 24, v + 0); // J'ai mis le premier dinosaure bleu
        drawCall.vertices.emplace_back(posB, animationIndex, v + 0);
        drawCall.vertices.emplace_back(posC, animationIndex + 24, v + 24);
        drawCall.vertices.emplace_back(posB, animationIndex, v + 0);
        drawCall.vertices.emplace_back(posC, animationIndex + 24, v + 24);
        drawCall.vertices.emplace_back(posD, animationIndex, v + 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, animationIndex, v + 0);
        drawCall.vertices.emplace_back(posB, animationIndex + 24, v + 0);
        drawCall.vertices.emplace_back(posC, animationIndex, v + 24);
        drawCall.vertices.emplace_back(posB, animationIndex + 24, v + 0);
        drawCall.vertices.emplace_back(posC, animationIndex, v + 24);
        drawCall.vertices.emplace_back(posD, animationIndex + 24, v + 24);
    }

    drawCall.scale = 1;
    drawCall.translation = playerPos;
    XDino_Draw(drawCall);
}

bool dino_player::IsAbove(dino_player& other)
{
    return playerPos.y < other.playerPos.y;
}

void dino_player::ApplyTerain(DinoVec2 a, DinoVec2 b)
{
    if (playerPos.y < a.y)
        playerPos.y = a.y;

    if (playerPos.y > b.y)
        playerPos.y = b.y;

    if (playerPos.x < a.x)
        playerPos.x = a.x;

    if (playerPos.x > b.x)
        playerPos.x = b.x;
}