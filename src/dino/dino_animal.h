#pragma once

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>
#include <dino/dino_entity.h>

class DinoAnimal : public DinoEntity {

    DinoVec2 m_dir = {};

    EAnimalKind m_kind = {};

    double m_timeAtBirth = 0;
    bool m_dead = false;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

    void ReactLimit(bool xChanged) override;
    void ReactLoop(double timeSinceStart);

public:
    void Init(EAnimalKind animalKind, DinoVec2 pos, double timeSinceStart);
    void Shut();

    void Update(float deltaTime);

    void Draw(double timeSinceStart);

    static void InitTexture();
    static void ShutTexture();

    static bool IsDead(DinoAnimal& animal);
};