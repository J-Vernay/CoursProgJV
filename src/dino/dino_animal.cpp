#include "dino_animal.h"

void dino_animal::DinoAnimal_Spawn(uint64_t textId_Animal, DinoVec2 animal_pos, int animal_index)
{
    textIdAnimal = textId_Animal;
    animalPos = animal_pos;
    animalIndex = animal_index;

    //debug
    timeAlive = 0;
}

void dino_animal::DinoAnimal_Update(float deltaTime)
{
    timeAlive += deltaTime;

    std::vector<DinoVertex> vs;
    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].color = DinoColor_WHITE;
    vs[1].pos = {1, 0};
    vs[1].color = DinoColor_WHITE;
    vs[2].pos = {0, 1};
    vs[2].color = DinoColor_WHITE;
    vs[3].pos = {1, 0};
    vs[3].color = DinoColor_WHITE;
    vs[4].pos = {0, 1};
    vs[4].color = DinoColor_WHITE;
    vs[5].pos = {1, 1};
    vs[5].color = DinoColor_WHITE;
    vs[0].u = 0;
    vs[0].v = 0;
    vs[1].u = 32;
    vs[1].v = 0;
    vs[2].u = 0;
    vs[2].v = 32;
    vs[3].u = 32;
    vs[3].v = 0;
    vs[4].u = 0;
    vs[4].v = 32;
    vs[5].u = 32;
    vs[5].v = 32;

    uint64_t vbufID_animal = XDino_CreateVertexBuffer(vs.data(), vs.size(), "animal");
    XDino_Draw(vbufID_animal, textIdAnimal, animalPos, 12);
    XDino_DestroyVertexBuffer(vbufID_animal);
}

void dino_animal::DinoAnimal_Despawn(std::vector<dino_animal>& animals)
{
    //debug
    DinoAnimal_Kill(animals);
}

void dino_animal::DinoAnimal_Kill(std::vector<dino_animal>& animals)
{
    animals.erase(animals.begin() + animalIndex);
}

bool dino_animal::DinoAnimal_ShouldDying()
{
    return timeAlive > 500;
}