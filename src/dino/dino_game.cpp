/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include "dino_player.h"

#include <algorithm>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<dino_player> dinoPlayers;

bool ComparePlayerPos(dino_player& a, dino_player& b)
{
    return a.IsAbove(b);
}

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    dinoPlayers.resize(4);
    dinoPlayers[0].Init(0, {windowSize.x / 2 - 100, windowSize.y / 2 - 100});
    dinoPlayers[1].Init(1, {windowSize.x / 2 - 100, windowSize.y / 2 + 100});
    dinoPlayers[2].Init(2, {windowSize.x / 2 + 100, windowSize.y / 2 - 100});
    dinoPlayers[3].Init(3, {windowSize.x / 2 + 100, windowSize.y / 2 + 100});
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    for (dino_player& player : dinoPlayers) {
        player.UpdatePlayer(deltaTime);
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    std::sort(dinoPlayers.begin(), dinoPlayers.end(), ComparePlayerPos);
    for (dino_player& player : dinoPlayers) {
        player.DrawPlayer(timeSinceStart);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Dessin du dinosaure que l'on peut bouger.
    {

    }

    // NOM Prénom
    {
        std::string text = "DUFOUR Enzo";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        drawCall.translation.x = windowSize.x - 2 * textSize.x;
        drawCall.translation.y = windowSize.y - 2 * textSize.y;
        XDino_Draw(drawCall);
    }
}

// Constantes.
constexpr float speed = 300.f; // Nombre de pixels parcourus en une seconde.

void dino_player::UpdatePlayer(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    this->isIdle = false;
    this->isWalking = false;
    this->isRunning = false;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        float playerSpeed = speed;

        if (gamepad.btn_right && !gamepad.btn_left) {
            playerSpeed *= 2.f;
        }

        this->playerPos.x += gamepad.stick_left_x * playerSpeed * deltaTime;
        this->playerPos.y += gamepad.stick_left_y * playerSpeed * deltaTime;

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
    }
}

void dino_player::DrawPlayer(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png"; // Ici on change en dinosaurs pour avoir accès au sprite sheet. 
    drawCall.vertices.reserve(6);

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};
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

    int v = indexPlayer * 24;

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

    drawCall.scale = 3;
    drawCall.translation = this->playerPos;
    XDino_Draw(drawCall);
}

bool dino_player::IsAbove(dino_player& other)
{
    return playerPos.y < other.playerPos.y;
}

void dino_player::Init(int index, DinoVec2 posInit)
{
    playerPos = posInit;
    indexPlayer = index;
}

void Dino_GameShut()
{

}