#pragma once

#include "dino_player.h"

#include <dino/xdino.h>

struct DinoAnimal : public DinoEntity {
    DinoVec2 m_dir = {};
    DinoVec2 m_targetPos = {};
    double m_spawnTime = 0;
    int m_animalType = 0;
    uint64_t m_texID;
    DinoVec2 m_posTopLeft = {};
    bool m_dead = false;

    void Init(double timeSinceStart, int animalIndex, uint64_t texID);
    void Update(float deltaTime, double timeSinceStart);
    void Draw(double timeSinceStart) override;
    void ReactLoop(double timeSinceStart) override;
    static bool IsDead(DinoAnimal& animal);
    void Shut();
    uint64_t GenerateVertexBuffer(double timeSinceStart, float alpha);
};

struct DinoAnimalSpawner {
    std::vector<DinoAnimal> m_animals;
    uint64_t m_texID;
    double m_timeSinceLastSpawn;
    void Init();
    void Update(float deltaTime, double timeSinceStart, double chrono);
    void Shut();
};