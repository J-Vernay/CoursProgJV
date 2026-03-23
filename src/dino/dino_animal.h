#pragma once

#include "dino_player.h"
#include <dino/dino_score.h>
#include <dino/xdino.h>

struct DinoAnimal : public DinoEntity {
public:
    DinoVec2 m_dir = {};
    DinoVec2 m_targetPos = {};
    double m_spawnTime = 0;
    int m_animalType = 0;
    uint64_t m_texID;
    DinoVec2 m_posTopLeft = {};
    bool m_dead = false;
    int pointsValue = 0;
    int catchPlayerId;
    DinoScoreManager* m_ScoreManager;
    float despawnTime = 1.0f;
    float timeDead = 0;


    DinoAnimal(double timeSinceStart, int animalIndex, DinoScoreManager& dinoScoreManager, uint64_t texID);
    void Update(float deltaTime, double timeSinceStart);
    void Draw(double timeSinceStart) override;
    void ReactLoop(double timeSinceStart, int lassoIndex) override;
    static bool IsDead(DinoAnimal& animal);
    void Shut();
    DinoVertexBuffer GenerateVertexBuffer(double timeSinceStart, float alpha);

private:


};

struct DinoAnimalSpawner {
    std::vector<DinoAnimal> m_animals;
    uint64_t m_texID;
    double m_timeSinceLastSpawn;
    DinoScoreManager* m_ScoreManager;
    void Init(DinoScoreManager& dinoScoreManager);
    void Update(float deltaTime, double timeSinceStart, double chrono);
    void Shut();
};