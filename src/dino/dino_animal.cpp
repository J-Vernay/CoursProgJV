#include "dino_terrain.h"

#include <algorithm>
#include <cmath>
#include <dino/xdino.h>
#include <dino/dino_animal.h>

constexpr float FADE_IN_DURATION = 0.5f;

uint64_t DinoAnimal::s_texID = 0;

void DinoAnimal::InitTexture()
{
    s_texID = XDino_CreateGpuTexture("animals.png");
}

void DinoAnimal::ShutTexture()
{
    XDino_DestroyGpuTexture(s_texID);
}


void DinoAnimal::Init(EAnimalKind animalKind, DinoVec2 pos, double timeSinceStart)
{
    m_pos = pos;
    m_dir = XDino_RandomUnitVec2();

    m_kind = animalKind;

    m_timeAtBirth = timeSinceStart;
}

void DinoAnimal::Update(float deltaTime, DinoTerrain terrain)
{
    constexpr float SPEED = 75.f;

    m_pos.x += m_dir.x * SPEED * deltaTime;
    m_pos.y += m_dir.y * SPEED * deltaTime;

    DinoVec2 pos = m_pos;
    DinoVec2 topLeftCorner = terrain.GetTopLeft();
    DinoVec2 bottomRightCorner = terrain.GetBottomRight();
    pos.x = std::clamp(pos.x, topLeftCorner.x, bottomRightCorner.x);
    pos.y = std::clamp(pos.y, topLeftCorner.y, bottomRightCorner.y);

    if (pos.x != m_pos.x) {
        m_dir.x = -m_dir.x;
    }

    if (pos.y != m_pos.y) {
        m_dir.y = -m_dir.y;
    }
}

void DinoAnimal::Draw(double timeSinceStart)
{
    EAnimalAnim anim;
    if (abs(m_dir.x) > abs(m_dir.y)) {
        if (m_dir.x > 0)
            anim = EAnimalAnim::Right;
        else
            anim = EAnimalAnim::Left;
    }
    else {
        if (m_dir.y > 0)
            anim = EAnimalAnim::Down;
        else
            anim = EAnimalAnim::Up;
    }

    float alpha = 255;
    if ((timeSinceStart - m_timeAtBirth) < FADE_IN_DURATION)
        alpha = std::lerp(0x00, 0xFF, (timeSinceStart - m_timeAtBirth) / FADE_IN_DURATION);

    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, m_kind, anim, timeSinceStart, alpha);

    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");

    // -16 / -28 to move the position to the feet and not the corner of the texture
    XDino_Draw(vbufID, s_texID, {m_pos.x - 16, m_pos.y - 28});

    XDino_DestroyVertexBuffer(vbufID);
}