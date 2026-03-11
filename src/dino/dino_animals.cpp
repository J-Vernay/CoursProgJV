#include "dino_animals.h"

#include "dino_draw_utils.h"

#include <algorithm>
#include <cmath>

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

    m_pos = DinoVec2(XDino_RandomUint32(112,345), XDino_RandomUint32(75,245));

}

DinoVec2 dino_animal::Redirect(DinoVec2 pos, DinoVec2 dir)
{
    DinoVec2 newDir;
    float rnd = XDino_RandomFloat(-1, 1);
    if (pos.x < 112 || pos.x > 345) {
        if (pos.x < 112)
            m_pos.x = 113;
        if (pos.x > 345)
            m_pos.x = 344;
        newDir = DinoVec2(-dir.x, rnd);
    }
    else {

        if (pos.y < 75)
            m_pos.y = 76;
        if (pos.y > 245)
            m_pos.y = 244;
        newDir = DinoVec2(rnd, -dir.y);
    }
    return newDir;
}

void dino_animal::Update(double deltaTime, double timeSinceStart)
{
    m_pos.x += m_dir.x * speed * deltaTime;
    m_pos.y += m_dir.y * speed * deltaTime;
    if (m_pos.x <= 112 || m_pos.x >= 345 || m_pos.y <= 75 || m_pos.y >= 245) {
        m_dir = Redirect(m_pos, m_dir);
    }
    uint8_t dirAnim = -1;

    if (std::abs(m_dir.x) < std::abs(m_dir.y)) {
        if (m_dir.y < 0) {
            dirAnim = 0;
        }
        else {
            dirAnim = 1;
        }
    }
    else if (std::abs(m_dir.x) >= std::abs(m_dir.y)) {
        if (m_dir.x < 0) {
            dirAnim = 2;
        }
        else {
            dirAnim = 3;
        }
    }
    Draw(timeSinceStart, dirAnim);
}

void dino_animal::Draw(double timeSinceStart, uint8_t dirAnim)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart, m_type, dirAnim);
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

uint64_t dino_animal::GenerateVertexBuffer(double time, uint32_t animalType, uint8_t dirAnim)
{
    std::vector<DinoVertex> vs;

    Dino_GenVertices_Animal(vs, static_cast<EAnimalKind>(animalType), static_cast<EAnimalAnim>(dirAnim), time);

    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "animal");
}

void dino_animal::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}