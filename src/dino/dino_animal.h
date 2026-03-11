#pragma once

#include <dino/xdino.h>
#include "dino_draw_utils.h"

class dino_animal {
public :
    void DinoAnimal_Spawn(DinoVec2 terrainTopLeft);
    void DinoAnimal_Update(double timeSinceStart, float deltaTime);
    void DinoAnimal_InstantDespawn(std::vector<dino_animal>& animals, int index);

    static void DinoAnimal_InitStatic();
    static void DinoAnimal_ShutStatic();

private :
    static uint64_t textIdAnimal;

    bool moving = true;
    DinoVec2 animalPos;
    DinoVec2 animalMovingDirection;

    EAnimalKind animalType;
    EAnimalAnim animalAnimDirection = EAnimalAnim::Up;

    void DinoAnimal_GetRandomPos();
    void DinoAnimal_GetRandomDirection();

    bool CanAddXValue(float xToAdd);
    bool CanAddYValue(float yToAdd);

    float apparitionTime = 0.2f;
    float timeAlive;

    DinoVec2 m_terrainTopLeft;
};