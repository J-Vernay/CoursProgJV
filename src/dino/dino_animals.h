#pragma once
#include "xdino.h"
#include <vector>

class dino_animal {
public:
    uint16_t speed = 40;
    uint64_t m_texID;
    void Init(uint64_t& texID, uint64_t spawnTime);
    void Draw(double timeSinceStart, uint8_t dirAnim);
    void Update(double deltaTime, double timeSinceStart);
    void Shut();

private:
    DinoVec2 m_pos = {200, 200};
    DinoVec2 m_dir = {};
    uint32_t m_type;
    uint64_t GenerateVertexBuffer(double timeSinceStart, uint32_t animalType, uint8_t dirAnim);
    DinoVec2 Redirect(DinoVec2 pos, DinoVec2 dir);
    uint64_t spawnTime;
};

#pragma once

class dino_animalsGenerator {
public:
    float spawnDelay = 1;
    uint64_t m_texID;
    std::pmr::vector<dino_animal> animals;
    void Init();
    void Update(double deltaTime, double timeSinceStart);
    void Shut();
};