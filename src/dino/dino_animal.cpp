#include "dino_terrain.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <dino/xdino.h>
#include <dino/dino_animal.h>

constexpr float FADE_IN_DURATION = 0.5f;
constexpr int SCORE_PER_ANIMAL = 10;
constexpr float SCORE_DISPLAY_TIME = 1.f;

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

    m_isInGame = true;
}

void DinoAnimal::Shut()
{

}

void DinoAnimal::Update(float deltaTime)
{
    if (m_scoreToGive != 0.f)
        return;

    constexpr float SPEED = 75.f;

    m_pos.x += m_dir.x * SPEED * deltaTime;
    m_pos.y += m_dir.y * SPEED * deltaTime;
}

EAnimalKind DinoAnimal::GetKind()
{
    if (m_scoreToGive != 0.f)
        return EAnimalKind::Other; // Don't count it if already looped

    return m_kind;
}

void DinoAnimal::ReactLimit(bool xChanged)
{
    if (xChanged) {
        m_dir.x = -m_dir.x;
    }
    else {
        m_dir.y = -m_dir.y;
    }
}

void DinoAnimal::ReactLoop(double timeSinceStart, int sameKindCount, DinoColor lassoColor)
{
    m_scoreToGive = sameKindCount * SCORE_PER_ANIMAL;
    m_timeToDisplayScore = SCORE_DISPLAY_TIME;
    m_scoreColor = lassoColor;
}


bool DinoAnimal::IsDead(DinoAnimal& animal)
{
    return animal.m_dead;
}

void DinoAnimal::Draw(double timeSinceStart)
{
    if (m_scoreToGive != 0.f)
        return;

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

    DinoVertexBuffer vbuf(vs.data(), vs.size(), "Animal");

    // -16 / -28 to move the position to the feet and not the corner of the texture
    XDino_Draw(vbuf.Get(), s_texID, {m_pos.x - 16, m_pos.y - 28});
}

void DinoAnimal::DrawScoreText(double deltaTime)
{
    if (m_timeToDisplayScore <= 0.f && m_scoreToGive == 0)
        return;

    if (m_timeToDisplayScore <= 0.f && m_scoreToGive != 0) {
        m_dead = true;
        return;
    }

    m_timeToDisplayScore -= deltaTime;

    uint8_t alpha = 0;
    alpha = std::lerp(0x00, 0xFF, std::clamp(m_timeToDisplayScore * 2 / SCORE_DISPLAY_TIME, (double)0, (double)1));

    m_scoreColor.a = alpha;

    {
        std::string text = std::format("+{}", m_scoreToGive);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, m_scoreColor, DinoColor_INVISIBLE, {(m_pos.x - 20) / 2, (m_pos.y - 26) / 2});
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "score");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }
}