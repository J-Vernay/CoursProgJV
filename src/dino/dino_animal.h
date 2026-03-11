#pragma once

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>

class DinoAnimal {

    DinoVec2 m_pos = {};
    DinoVec2 m_dir = {};

    EAnimalKind m_kind = {};

    double m_timeAtBirth = 0;

    static uint64_t s_texID;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(EAnimalKind animalKind, DinoVec2 pos, double timeSinceStart);
    void Update(float deltaTime, DinoTerrain terrain);
    void Draw(double timeSinceStart);

    static void InitTexture();
    static void ShutTexture();
};