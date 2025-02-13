#include "DinoPlayer.h"

#include <iostream>

DinoPlayer::DinoPlayer()
{

}

void DinoPlayer::Initialize(DinoGamepadIdx gamepadIdx, DinoVec2 pos)
{
    m_gamepadIdx = gamepadIdx;
    std::cout << (int)gamepadIdx;
    m_pos = pos;
    m_movementSpeed = 300;
}

void DinoPlayer::Update(float deltaTime)
{
    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(m_gamepadIdx, gamepad);
    if (!bSuccess)
        return;

    float playerSpeed = m_movementSpeed;

    if (gamepad.btn_right && !gamepad.btn_left) {
        playerSpeed *= 2.0f;
    }

    DinoVec2 offset = {gamepad.stick_left_x * playerSpeed * deltaTime, gamepad.stick_left_y * playerSpeed * deltaTime};

    m_pos.x += offset.x;
    m_pos.y += offset.y;

    m_isMirror = offset.x < 0;

    if (gamepad.stick_left_x == 00 && gamepad.stick_left_y == 0) {
        m_state = Idle;
    }
    else if (gamepad.btn_right) {
        m_state = Running;
    }
    else {
        m_state = Walking;
    }
}

void DinoPlayer::Draw(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    drawCall.vertices.reserve(6);

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};
    DinoColor color = DinoColor_WHITE;

    int spriteOffsetY = static_cast<int>(m_gamepadIdx) * 24;

    if (m_state == Idle) {
        m_animationIndex = 0;
        m_animationIndex = int(timeSinceStart / 0.125f) % 4 * 24;
    }
    if (m_state == Walking) {
        m_animationIndex = 96;
        m_animationIndex += int(timeSinceStart / 0.125f) % 6 * 24;
    }
    if (m_state == Running) {
        m_animationIndex = 432;
        m_animationIndex += int(timeSinceStart / 0.0625f) % 6 * 24;
    }

    if (m_isMirror) {
        drawCall.vertices.emplace_back(posA, m_animationIndex + 24, spriteOffsetY);
        drawCall.vertices.emplace_back(posB, m_animationIndex, spriteOffsetY);
        drawCall.vertices.emplace_back(posC, m_animationIndex + 24, spriteOffsetY + 24);
        drawCall.vertices.emplace_back(posB, m_animationIndex, spriteOffsetY);
        drawCall.vertices.emplace_back(posC, m_animationIndex + 24, spriteOffsetY + 24);
        drawCall.vertices.emplace_back(posD, m_animationIndex, spriteOffsetY + 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, m_animationIndex, spriteOffsetY);
        drawCall.vertices.emplace_back(posB, m_animationIndex + 24, spriteOffsetY);
        drawCall.vertices.emplace_back(posC, m_animationIndex, spriteOffsetY + 24);
        drawCall.vertices.emplace_back(posB, m_animationIndex + 24, spriteOffsetY);
        drawCall.vertices.emplace_back(posC, m_animationIndex, spriteOffsetY + 24);
        drawCall.vertices.emplace_back(posD, m_animationIndex + 24, spriteOffsetY + 24);
    }

    drawCall.translation = m_pos;
    XDino_Draw(drawCall);
}