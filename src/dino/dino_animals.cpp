#include "dino_animals.h"

uint64_t Animal::GenerateVertexBuffer()
{
    std::vector<DinoVertex> vs;

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = 0;
    vs[0].v = 0;
    vs[1].pos = {32, 0};
    vs[1].u = 32;
    vs[1].v = 0;
    vs[2].pos = {0, 32};
    vs[2].u = 0;
    vs[2].v = 32;
    vs[3].pos = {32, 0};
    vs[3].u = 32;
    vs[3].v = 0;
    vs[4].pos = {0, 32};
    vs[4].u = 0;
    vs[4].v = 32;
    vs[5].pos = {32, 32};
    vs[5].u = 32;
    vs[5].v = 32;

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");
}

void Animal::Init()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    m_pos = {windowSize.x / 2, windowSize.y / 2};
    m_texID = XDino_CreateGpuTexture("animals.png");
}

void Animal::Update(double timeSinceStart, float deltaTime, DinoVec2 direction)
{

}

void Animal::Draw(int indexAnimal)
{
    uint64_t vbufID = GenerateVertexBuffer();
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

void Animal::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}