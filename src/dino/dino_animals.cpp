#include "dino_animals.h"

constexpr float ANIMAL_SPEED = 10;
constexpr DinoVec2 COUNT = {8, 6};
constexpr DinoVec2 SIZE = {COUNT.x * 32, COUNT.y * 32};

uint64_t Animal::GenerateVertexBuffer()
{
    std::vector<DinoVertex> vs;
    uint16_t ubase = m_idxAnimal * 128;
    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = ubase ;
    vs[0].v = 0;
    vs[1].pos = {32, 0};
    vs[1].u = ubase +32;
    vs[1].v = 0;
    vs[2].pos = {0, 32};
    vs[2].u = ubase;
    vs[2].v = 32;
    vs[3].pos = {32, 0};
    vs[3].u = ubase + 32;
    vs[3].v = 0;
    vs[4].pos = {0, 32};
    vs[4].u = ubase;
    vs[4].v = 32;
    vs[5].pos = {32, 32};
    vs[5].u = ubase + 32;
    vs[5].v = 32;

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");
}

void Animal::Init()
{
    float dx = (480 - SIZE.x) / 2;
    float dy = (360 - SIZE.y) / 2;
    m_idxAnimal = XDino_RandomInt32(0, 7);
    m_pos = {XDino_RandomFloat(dx, 368), XDino_RandomFloat(dy, 256)};
    m_targetPos = {XDino_RandomFloat(dx, 368), XDino_RandomFloat(dy, 256)};
    m_texID = XDino_CreateGpuTexture("animals.png");
}

void Animal::Update(float deltaTime)
{
    float dx = (480 - SIZE.x) / 2;
    float dy = (360 - SIZE.y) / 2;
    float directionX = m_targetPos.x - m_pos.x;
    float directionY =  m_targetPos.y - m_pos.y;
    float dist = hypotf(directionX, directionY);

    if(dist < 4) {
        m_targetPos = {XDino_RandomFloat(dx, 368), XDino_RandomFloat(dy, 256)};
        return;
    }

    m_pos.x += (directionX / dist) * ANIMAL_SPEED * deltaTime;
    m_pos.y += (directionY / dist) * ANIMAL_SPEED * deltaTime;
    
}

void Animal::Draw()
{
    uint64_t vbufID = GenerateVertexBuffer();
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

void Animal::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}