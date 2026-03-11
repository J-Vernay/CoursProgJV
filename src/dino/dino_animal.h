#pragma once

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>


class DinoAnimal {
private:
    DinoVec2 m_pos = {};
    DinoVec2 m_dir = {};
    EAnimalKind m_kind = {};
    uint64_t m_texID = {};

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(EAnimalKind animal, DinoVec2 pos, uint64_t texID);
    void Update(double timeSinceStart, float deltaTime);
    void Draw(double timeSinceStart);
    void Shut();
};