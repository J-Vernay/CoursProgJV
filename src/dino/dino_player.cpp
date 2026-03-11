#include "dino_terrain.h"

#include <__msvc_ostream.hpp>
#include <iostream>
#include <dino/dino_player.h>

#include <dino/xdino.h>

constexpr float DINO_SPEED = 150.f;

void DinoPlayer::Init(int idxPlayer)
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    m_pos = {windowSize.x / 2, windowSize.y / 2};
    m_texID = XDino_CreateGpuTexture("dinosaurs.png");
    m_idxPlayer = idxPlayer;
}

void DinoPlayer::ResolveCollision(DinoPlayer& playerA, DinoPlayer& playerB)
{
    DinoVec2& a = playerA.m_pos;
    DinoVec2& b = playerB.m_pos;
    float ab = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    if (ab == 0 || ab >= 16)
        return;
    float dx = (16 - ab) / (2 * ab) * (b.x - a.x);
    float dy = (16 - ab) / (2 * ab) * (b.y - a.y);
    a.x -= dx;
    a.y -= dy;
    b.x += dx;
    b.y += dy;
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime, DinoTerrain terrain, DinoGamepad gamepad)
{
    m_bPressedRun = false;
    m_bMoving = false;

    float moveX;
    float moveY;
    float speed = DINO_SPEED;
    if (gamepad.btn_right) {
        speed = DINO_SPEED * 2;
        m_bPressedRun = true;
    }
    if (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
        m_bMoving = true;

    if ((m_pos.x > terrain.m_downRight.x) && gamepad.stick_left_x > 0) {
        moveX = 0;
    }
    else if ((m_pos.x < terrain.m_topLeft.x) && gamepad.stick_left_x < 0) {
        moveX = 0;
    }
    else {
        moveX = gamepad.stick_left_x;
    }

    if ((m_pos.y > terrain.m_downRight.y) && gamepad.stick_left_y > 0) {
        moveY = 0;
    }
    else if ((m_pos.y < terrain.m_topLeft.y) && gamepad.stick_left_y < 0) {
        moveY = 0;
    }
    else {
        moveY = gamepad.stick_left_y;
    }

    // if (m_pos.y > terrain.m_downRight.y && m_pos.y < terrain.m_topLeft.y) {
    //     gamepad.stick_left_y = 0;
    // }

    if (timeSinceStart >= m_endHitAnim) {
        m_pos.x += moveX * speed * deltaTime;
        m_pos.y += moveY * speed * deltaTime;
    }

    if (gamepad.stick_left_x < 0)
        m_bLeft = true;
    if (gamepad.stick_left_x > 0)
        m_bLeft = false;

    if (gamepad.btn_left) {
        m_endHitAnim = timeSinceStart + 3;
    }
}

void DinoPlayer::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}

void DinoPlayer::Draw(double timeSinceStart)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart);
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

uint64_t DinoPlayer::GenerateVertexBuffer(double timeSinceStart)
{
    float animSpeed;
    int frameCount;
    int ubase;
    if (timeSinceStart < m_endHitAnim) {
        // ANIM HIT
        animSpeed = 8;
        frameCount = 3;
        ubase = 336;
    }
    else if (m_bMoving) {
        if (m_bPressedRun) {
            // ANIM RUN
            animSpeed = 16;
            frameCount = 6;
            ubase = 432;
        }
        else {
            // ANIM WALK
            animSpeed = 8;
            frameCount = 6;
            ubase = 96;
        }
    }
    else {
        // ANIM IDLE
        animSpeed = 8;
        frameCount = 4;
        ubase = 0;
    }

    int uAnim = (static_cast<int>(timeSinceStart * animSpeed) % frameCount) * 24 + ubase;

    std::vector<DinoVertex> vs;
    uint16_t umin, umax;
    if (m_bLeft) {
        umin = uAnim + 24;
        umax = uAnim + 0;
    }
    else {
        umin = uAnim + 0;
        umax = uAnim + 24;
    }

    uint16_t vbase = 24 * m_idxPlayer;

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin;
    vs[0].v = vbase;
    vs[1].pos = {24, 0};
    vs[1].u = umax;
    vs[1].v = vbase;
    vs[2].pos = {0, 24};
    vs[2].u = umin;
    vs[2].v = vbase + 24;
    vs[3].pos = {24, 0};
    vs[3].u = umax;
    vs[3].v = vbase;
    vs[4].pos = {0, 24};
    vs[4].u = umin;
    vs[4].v = vbase + 24;
    vs[5].pos = {24, 24};
    vs[5].u = umax;
    vs[5].v = vbase + 24;
    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
}