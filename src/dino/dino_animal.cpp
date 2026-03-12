#include <dino/dino_animal.h>

uint64_t DinoAnimal::s_texID = 0;

void DinoAnimal::Init(double timeSinceStart, EAnimalKind animal, DinoVec2 pos)
{
    m_pos = pos;
    m_dir = XDino_RandomUnitVec2();
    m_kind = animal;
    m_spawnTime = timeSinceStart;
}

void DinoAnimal::Update(double timeSinceStart, float deltaTime)
{
    constexpr float SPEED = 40; // px/s
    m_pos.x += m_dir.x * SPEED * deltaTime;
    m_pos.y += m_dir.y * SPEED * deltaTime;
}

void DinoAnimal::ReactLimit()
{
    m_dir = XDino_RandomUnitVec2();
}

void DinoAnimal::ReactLoop(double timeSinceStart)
{

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

    constexpr float TIME_FADEIN = 1;
    double timeAlive = timeSinceStart - m_spawnTime;
    uint8_t alpha = 255;
    if (timeAlive < TIME_FADEIN)
        alpha = (timeAlive * 255) / TIME_FADEIN;

    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, m_kind, anim, timeSinceStart);
    for (DinoVertex& v : vs)
        v.color = {255, 255, 255, alpha};
    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");
    DinoVec2 drawPos = {m_pos.x - 16, m_pos.y - 32};
    XDino_Draw(vbufID, s_texID, drawPos);
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