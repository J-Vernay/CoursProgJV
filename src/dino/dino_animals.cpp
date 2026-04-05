#include "dino_animals.h"
#include "dino_draw_utils.h"

constexpr float ANIMAL_SPEED = 100;
constexpr DinoVec2 COUNT = {8, 6};
constexpr DinoVec2 SIZE = {COUNT.x * 32, COUNT.y * 32};


void Animal::Init()
{
    float dx = (480 - SIZE.x) / 2;
    float dy = (360 - SIZE.y) / 2;
    m_idxAnimal = XDino_RandomInt32(0, 7);
    m_kind = static_cast<EAnimalKind>(m_idxAnimal);
    m_moveSpeed = XDino_RandomFloat(ANIMAL_SPEED - 2, ANIMAL_SPEED + 2);
    m_pos = {XDino_RandomFloat(dx, 350), XDino_RandomFloat(dy, 250)};
    m_targetPos = {XDino_RandomFloat(dx, 350), XDino_RandomFloat(dy, 250)};
    m_texID = XDino_CreateGpuTexture("animals.png");
}

void Animal::Update(float deltaTime, double timeSinceStart)
{

    float dx = (480 - SIZE.x) / 2;
    float dy = (360 - SIZE.y) / 2;
    float directionX = m_targetPos.x - m_pos.x;
    float directionY = m_targetPos.y - m_pos.y;
    float dist = hypotf(directionX, directionY);

    if (std::abs(directionX) > std::abs(directionY)) {
        if (directionX > 0)
            m_anim = EAnimalAnim::Right;
        else
            m_anim = EAnimalAnim::Left;
    }
    else if (std::abs(directionY) > std::abs(directionX)) {
        if (directionY > 0)
            m_anim = EAnimalAnim::Down;
        else
            m_anim = EAnimalAnim::Up;
    }

    Draw(timeSinceStart);

    if (dist < 1) {
        m_targetPos = {XDino_RandomFloat(dx, 350), XDino_RandomFloat(dy, 250)};
        return;
    }

    m_pos.x += (directionX / dist) * m_moveSpeed * deltaTime;
    m_pos.y += (directionY / dist) * m_moveSpeed * deltaTime;
}

void Animal::Draw(double timeSinceStart)
{
    constexpr float TIMEFADEIN = 1;
    double timeAlive = timeSinceStart - m_spawnTime;
    uint8_t alpha = 255;
    if (timeAlive < TIMEFADEIN) {
        alpha = (timeAlive * 255) / TIMEFADEIN;
    }

    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, m_kind, m_anim, timeSinceStart);
    for (DinoVertex& v : vs) {
        v.color = {255, 255, 255, alpha};
    }

    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

void Animal::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}
