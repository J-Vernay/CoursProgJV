#pragma once

#include "dino_player.h"

#include <dino/xdino.h>

struct DinoAnimal {
    DinoVec2 m_pos = {};
    DinoVec2 m_dir = {};
    bool m_bRight = false;
    int m_animalType = 0;
    uint64_t m_texID;
    DinoVec2 m_posTopLeft = {};

    void Init(int animalIndex, uint64_t texID);
    void Update(double timeSinceStart);
    void Draw(double timeSinceStart);
    uint64_t GenerateVertexBuffer(double timeSinceStart);
};

struct DinoAnimalSpawner {
    std::vector<DinoAnimal> m_animals;
    uint64_t m_texID;
    double m_timeSinceLastSpawn;
    void Init();
    void Update(double timeSinceStart);
    void Shut();
};