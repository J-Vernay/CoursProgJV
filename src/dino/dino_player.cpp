#include <dino/xdino.h>
#include "dino_player.h"
#include <dino/dino_draw_utils.h>

// Constantes.
constexpr float SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void dino_player::Init(DinoVec2 posInit, int index, DinoGamepadIdx idxGamepad, DinoColor colorLasso)
{
    pos = posInit;
    indexPlayer = index;
    indexGamepad = idxGamepad;
    lassoColor = colorLasso;
}

void dino_player::Update(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    isIdle = false;
    isWalking = false;
    isRunning = false;

    DinoGamepad gamepad;

    bool bSuccess = XDino_GetGamepad(DinoGamepadIdx_ALL[indexPlayer], gamepad);
    if (!bSuccess)
        gamepad = {}; // Laisser vide, on considère le joueur immobile

    float playerSpeed = SPEED;

    if (gamepad.btn_right) {
        playerSpeed *= 2.f;
    }

    this->pos.x += gamepad.stick_left_x * playerSpeed * deltaTime;
    this->pos.y += gamepad.stick_left_y * playerSpeed * deltaTime;

    if (gamepad.stick_left_x != 0) {
        this->isMirror = gamepad.stick_left_x < 0;
    }

    this->isIdle = gamepad.stick_left_x == 00 && gamepad.stick_left_y == 0;
    if (!this->isIdle) {
        if (gamepad.btn_right)
            this->isRunning = true;
        else {
            this->isWalking = true;
        }
    }

    lasso.emplace_back(pos);
    if (lasso.size() > 120) {
        lasso.erase(lasso.begin());
    }
}

void dino_player::DrawLasso()
{
    // Lasso
    DinoDrawCall drawCallLasso = Dino_CreateDrawCall_Polyline(lasso, 4, lassoColor);
    XDino_Draw(drawCallLasso);
}

void dino_player::Draw(double timeSinceStart)
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

    if (this->isIdle) {
        int indexFrame = int(timeSinceStart * 8) % 4;
        animationIndex = 0 + 24 * indexFrame;
    }
    if (this->isWalking) {
        int indexFrame = int(timeSinceStart * 8) % 6;
        animationIndex = 96 + 24 * indexFrame;
    }
    if (this->isRunning) {
        int indexFrame = int(timeSinceStart * 16) % 6;
        animationIndex = 432 + 24 * indexFrame;
    }

    int v = indexPlayer * 24; // 24 pixels par ligne de sprite

    if (this->isMirror) {
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
    drawCall.translation = this->pos;
    XDino_Draw(drawCall);
}

bool dino_player::IsAbove(dino_player& other)
{
    return pos.y < other.pos.y;
}