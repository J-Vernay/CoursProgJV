#pragma once

#include "dino_Entity.h"
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

//forward declaration
class DinoGameState;

class dino_animal : public dino_Entity {
public :
    bool wasCatched = false;

    dino_animal(DinoGameState& dino_game_state, float collisionRadius);
    void Update(float deltaTime) override;

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

    DinoVec2 popUPosition;
    int givenPoints;
    int catchingPlayerId;

    EAnimalKind animalType;
    EAnimalAnim animalAnimDirection = EAnimalAnim::Up;

    void DinoAnimal_GetRandomPos();
    void DinoAnimal_GetRandomDirection();

    const float apparitionTime = 0.2f;
    const float despawnTime = 1.0f;
    float timeAlive;

    DinoGameState* m_dinoGameState;
};