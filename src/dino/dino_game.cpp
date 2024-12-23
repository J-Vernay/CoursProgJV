/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "xdino.h"

// Variables globales.
double rotation = 360.0;
double scale = 1.0;

void Dino_GameInit()
{

}

void Dino_GameUpdate(double deltaTime)
{
    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(DinoGamepad_KEYBOARD, gamepad);
    if (!bSuccess)
        DINO_CRITICAL("Cannot read keyboard state");

    if (gamepad.btn_down && !gamepad.btn_up)
        scale /= 1.01;
    if (gamepad.btn_up && !gamepad.btn_down)
        scale *= 1.01;
    if (gamepad.btn_left && !gamepad.btn_right)
        rotation += 90.0 * deltaTime;
    if (gamepad.btn_right && !gamepad.btn_left)
        rotation -= 90.0 * deltaTime;
}

void Dino_GameDraw()
{
    constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
    constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
    constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
    constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

    DinoVec2 size = XDino_GetWindowSize();
    int32_t quarterWidth = size.x / 4;
    int32_t quarterHeight = size.y / 4;

    DinoDrawCall drawCall;
    drawCall.vertices.resize(6);
    drawCall.vertices[0].pos = {-quarterWidth, -quarterHeight};
    drawCall.vertices[0].color = PURPLE;
    drawCall.vertices[1].pos = {quarterWidth, -quarterHeight};
    drawCall.vertices[1].color = CYAN;
    drawCall.vertices[2].pos = {-quarterWidth, quarterHeight};
    drawCall.vertices[2].color = PINK;
    drawCall.vertices[3].pos = {quarterWidth, -quarterHeight};
    drawCall.vertices[3].color = CYAN;
    drawCall.vertices[4].pos = {-quarterWidth, quarterHeight};
    drawCall.vertices[4].color = PINK;
    drawCall.vertices[5].pos = {quarterWidth, quarterHeight};
    drawCall.vertices[5].color = ORANGE;

    drawCall.translation = {size.x / 2, size.y / 2};
    drawCall.rotation = rotation;
    drawCall.scale = scale;

    drawCall.vertices[0].u = 0;
    drawCall.vertices[0].v = 0;
    drawCall.vertices[1].u = 96;
    drawCall.vertices[1].v = 0;
    drawCall.vertices[2].u = 0;
    drawCall.vertices[2].v = 96;
    drawCall.vertices[3].u = 96;
    drawCall.vertices[3].v = 0;
    drawCall.vertices[4].u = 0;
    drawCall.vertices[4].v = 96;
    drawCall.vertices[5].u = 96;
    drawCall.vertices[5].v = 96;
    drawCall.textureName = "monogram-bitmap.png";

    XDino_Draw(drawCall);
}

void Dino_GameShut()
{

}