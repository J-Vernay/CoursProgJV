#include <iostream>
#include <dino/dino_animal.h>

uint64_t DinoAnimal::s_texID = 0;

void DinoAnimal::Init(EAnimalKind animal, DinoVec2 pos)
{
    m_pos = pos;
    m_dir = XDino_RandomUnitVec2();
    m_kind = animal;
    m_timer = 10;
}

void DinoAnimal::Update(double timeSinceStart, float deltaTime)
{
    // m_timer -= deltaTime;
    // if (m_timer <= 0) {
    //     m_timer = 1;
    //     m_dir = XDino_RandomUnitVec2();
    //     std::cout << m_timer << std::endl;
    // }

    constexpr float SPEED = 50; // px/s
    m_pos.x += m_dir.x * SPEED * deltaTime;
    m_pos.y += m_dir.y * SPEED * deltaTime;

    ApplyLimit();
}

void DinoAnimal::ApplyLimit(DinoVec2 min, DinoVec2 max)
{
    DinoVec2 pos = m_pos;
    if (pos.x > max.x)
        pos.x = max.x;
    if (pos.y > max.y)
        pos.y = max.y;
    if (pos.x < min.x)
        pos.x = min.x;
    if (pos.y < min.y)
        pos.y = min.y;

    if (pos.x != m_pos.x || pos.y != m_pos.y) {
        m_pos = pos;
        m_dir = XDino_RandomUnitVec2();
    }
}

void DinoAnimal::Draw(double timeSinceStart)
{
    EAnimalAnim anim;
    if (abs(m_dir.x) > abs(m_dir.y)) {
        // Mouvement horizontal
        if (m_dir.x > 0)
            anim = EAnimalAnim::Right;
        else
            anim = EAnimalAnim::Left;
    }
    else {
        // Mouvement vertical
        if (m_dir.y > 0)
            anim = EAnimalAnim::Down;
        else
            anim = EAnimalAnim::Up;
    }

    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, m_kind, anim, timeSinceStart);
    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");
    XDino_Draw(vbufID, s_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoAnimal::Shut()
{
}

void DinoAnimal::InitStatic()
{
    s_texID = XDino_CreateGpuTexture("animals.png");
}

void DinoAnimal::ShutStatic()
{
    XDino_DestroyGpuTexture(s_texID);
}