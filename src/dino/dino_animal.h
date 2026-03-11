#pragma once

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>


class DinoAnimal {
private:
    DinoVec2 m_pos = {};
    DinoVec2 m_dir = {};
    EAnimalKind m_kind = {};
    double m_spawnTime = 0;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(double timeSinceStart, EAnimalKind animal, DinoVec2 pos);
    void Update(double timeSinceStart, float deltaTime);
    void Draw(double timeSinceStart);
    void Shut();

    static void InitStatic();
    static void ShutStatic();
};