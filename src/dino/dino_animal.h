#pragma once

#include "dino_player.h"
#include <dino/dino_score.h>
#include <dino/xdino.h>

struct DinoAnimal : public DinoEntity {

public:
    DinoAnimal(double timeSinceStart, int animalIndex, DinoScoreManager& dinoScoreManager, uint64_t texID);
    void Update(float deltaTime, double timeSinceStart);
    void Draw(double timeSinceStart) override;
    void ReactLoop(double timeSinceStart, int lassoIndex) override;
    static bool IsDead(DinoAnimal& animal);
    static void ShutStatic();

private:
    DinoVec2 m_dir = {};
    DinoVec2 m_targetPos = {};
    DinoVec2 m_posTopLeft = {};

    int m_animalType = 0;
    double m_spawnTime = 0;
    int m_pointsValue = 0;
    int m_catchPlayerId;
    bool m_dead = false;

    uint64_t m_texID;

    float m_despawnTime = 1.0f;
    float m_timeDead = 0;

    DinoScoreManager* m_ScoreManager;
    DinoVertexBuffer GenerateVertexBuffer(double timeSinceStart, float alpha);
};

struct DinoAnimalSpawner {

public:
    std::vector<DinoAnimal> m_animals;

    void Init(DinoScoreManager& dinoScoreManager);
    void Update(float deltaTime, double timeSinceStart, double chrono);
    void Shut();

private:
    uint64_t m_texID;
    double m_timeSinceLastSpawn;

    DinoScoreManager* m_ScoreManager;

};