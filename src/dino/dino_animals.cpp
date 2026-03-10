#include "dino_animals.h"

float delaySinceLastSpawn;

void dino_animalsGenerator::Init()
{
    m_texID = XDino_CreateGpuTexture("animals.png");
    animals.emplace_back();
    dino_animal& animal = animals.back();
    animal.Init(m_texID);

}

void dino_animalsGenerator::Update(double deltaTime, double timeSinceStart)
{

    delaySinceLastSpawn += deltaTime;
    if (delaySinceLastSpawn >= spawnDelay) {
        animals.emplace_back();
        dino_animal& animal = animals.back();
        animal.Init(m_texID);
        delaySinceLastSpawn = 0;
    }
    for (dino_animal& animal : animals) {
        animal.Update(deltaTime, timeSinceStart);
    }
}

void dino_animalsGenerator::Shut()
{
    for (auto& animal : animals) {
        animal.Shut();
    }
}

void dino_animal::Init(uint64_t& texID)
{
    m_texID = texID;
    uint32_t rnd = XDino_RandomInt32(0, 7);
    m_type = rnd;

    while (m_dir.x == 0 && m_dir.y == 0) {
        m_dir.x = XDino_RandomInt32(-1, 1);
        m_dir.y = XDino_RandomInt32(-1, 1);
    }
    if (m_dir.x != 0 && m_dir.y != 0) {
        m_dir.x /= 2;
        m_dir.y /= 2;
    }

}

void dino_animal::Update(double deltaTime, double timeSinceStart)
{
    m_pos.x += m_dir.x * speed * deltaTime;
    m_pos.y += m_dir.y * speed * deltaTime;
    Draw(timeSinceStart);
}

void dino_animal::Draw(double timeSinceStart)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart, m_type);
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

uint64_t dino_animal::GenerateVertexBuffer(double time, uint32_t animalType)
{
    uint64_t base = 128 * animalType;
    uint16_t umin = 0;
    uint16_t umax = 32;
    std::vector<DinoVertex> vs;

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin + base;
    vs[0].v = 0;
    vs[1].pos = {32, 0};
    vs[1].u = umax + base;
    vs[1].v = 0;
    vs[2].pos = {0, 32};
    vs[2].u = umin + base;
    vs[2].v = 32;
    vs[3].pos = {32, 0};
    vs[3].u = umax + base;
    vs[3].v = 0;
    vs[4].pos = {0, 32};
    vs[4].u = umin + base;
    vs[4].v = 32;
    vs[5].pos = {32, 32};
    vs[5].u = umax + base;
    vs[5].v = 32;

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "animal");
}

void dino_animal::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}