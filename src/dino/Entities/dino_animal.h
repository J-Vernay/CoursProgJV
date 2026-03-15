#pragma once

#include "dino_Entity.h"

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

class dino_animal : public dino_Entity {
public :
    bool wasCatched = false;

    dino_animal(DinoVec2 terrainTopLeft, float collisionRadius);
    //void DinoAnimal_Spawn(DinoVec2 terrainTopLeft, float collisionRadius);
    void Update(float deltaTime) override;
    void DinoAnimal_InstantDespawn(std::vector<dino_Entity>& entities, int index);

    static void DinoAnimal_InitStatic();
    static void DinoAnimal_ShutStatic();
    bool IsEntityDead() override;
    void ReactionToBorderCross() override;
    void DrawEntity(double timeSinceStart) override;
    void LassoCatched(int playerId) override;

private :
    static uint64_t textIdAnimal;

    bool moving = true;
    DinoVec2 animalMovingDirection;

    EAnimalKind animalType;
    EAnimalAnim animalAnimDirection = EAnimalAnim::Up;

    void DinoAnimal_GetRandomPos();
    void DinoAnimal_GetRandomDirection();

    float apparitionTime = 0.2f;
    float timeAlive;

    DinoVec2 m_terrainTopLeft;
};