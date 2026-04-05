#pragma once

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

class DinoAnimal : public DinoEntity {

    DinoVec2 m_dir = {};

    EAnimalKind m_kind = {};
    int m_scoreToGive = 0;
    double m_timeToDisplayScore = 0;
    DinoColor m_scoreColor = {};

    double m_timeAtBirth = 0;
    bool m_dead = false;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

    void ReactLimit(bool xChanged) override;
    void ReactLoop(double timeSinceStart, int score, DinoColor lassoColor) override;

public:
    void Init(EAnimalKind animalKind, DinoVec2 pos, double timeSinceStart);
    void Shut();

    void Update(float deltaTime);

    EAnimalKind GetKind() override;

    void Draw(double timeSinceStart);
    void DrawScoreText(double deltaTime);

    static void InitTexture();
    static void ShutTexture();

    static bool IsDead(DinoAnimal& animal);
};