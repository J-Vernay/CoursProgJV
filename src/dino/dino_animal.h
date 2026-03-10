#pragma once

#include <dino/xdino.h>

class dino_animal {
public :
    void DinoAnimal_Spawn(uint64_t textId_Animal, DinoVec2 animal_pos, int animal_index);
    void DinoAnimal_Update(float deltaTime);
    void DinoAnimal_Despawn(std::vector<dino_animal>& animals);
    bool DinoAnimal_ShouldDying();

private :
    uint64_t textIdAnimal;
    int animalIndex;

    bool moving = true;
    DinoVec2 animalPos;

    //debug variables
    float timeAlive;
    //

    void DinoAnimal_Kill(std::vector<dino_animal>& animals);
};