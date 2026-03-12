#pragma once

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>


class DinoAnimal {
    DinoVec2 m_pos = {};
    DinoVec2 m_dir = {};
    EAnimalKind m_kind = {};
    float m_timer = {};

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(EAnimalKind animal, DinoVec2 pos);
    void Update(double timeSinceStart, float deltaTime);
    void ApplyLimit(DinoVec2 min, DinoVec2 max);
    void Draw(double timeSinceStart);
    void Shut();

    static void InitStatic();
    static void ShutStatic();
};