#pragma once
#include "dino_actor.h"
#include "dino_animation.h"

#include <dino/xdino.h>

class DinoAnimal : public DinoActor {
public:
    enum AnimalType {
        PIG_1,
        PIG_2,
        COW_1,
        COW_2,
        SHEEP_1,
        SHEEP_2,
        OSTRICH_1,
        OSTRICH_2,
        MAX
    };

private:
    enum AnimationId {
        WALK_SIDE,
        WALK_DOWN,
        WALK_UP,
        DEAD
    };

    DinoVec2 direction;
    AnimalType type;
    AnimatorState<AnimationId> animatorState;

public:
    explicit DinoAnimal(DinoVec2 position)
        : DinoActor(position), type(static_cast<AnimalType>(XDino_RandomInt32(0, MAX - 1))),
          animatorState({0, 0, WALK_SIDE})
    {
        choose_random_direction();
    }

    void update(float deltaTime) override;
    void draw() const override;

private:
    void update_movement(float deltaTime);
    void update_animation(float deltaTime);
    void choose_random_direction();
};