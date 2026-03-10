#pragma once

#include "dino_player.h"

#include <dino/xdino.h>

struct DinoAnimal {
    DinoVec2 m_pos = {};
    bool m_bRight = false;
    int m_animalType = 0;
    uint64_t m_texID;

    void Init(int animalIndex);
    void Draw(double timeSinceStart);
    void Shut();
    uint64_t GenerateVertexBuffer(double timeSinceStart);
};