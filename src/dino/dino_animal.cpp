#include <dino/dino_animal.h>

void DinoAnimal::Init(EAnimalKind animal, DinoVec2 pos, uint64_t texID)
{
    m_pos = pos;
    m_dir = XDino_RandomUnitVec2();
    m_kind = animal;
    m_texID = texID;
}

void DinoAnimal::Update(double timeSinceStart, float deltaTime)
{
    constexpr float SPEED = 150; // px/s
    m_pos.x += m_dir.x * SPEED * deltaTime;
    m_pos.y += m_dir.y * SPEED * deltaTime;
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
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoAnimal::Shut()
{
}