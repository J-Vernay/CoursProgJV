#pragma once
#include <dino/xdino.h>

class DinoAnimal {
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
    DinoVec2 position;
    bool direction;
    AnimalType type;

public:
    explicit DinoAnimal(DinoVec2 position) : position(position), direction(false),
                                             type(static_cast<AnimalType>(XDino_RandomInt32(0, MAX - 1)))
    {
    }

    void update(float deltaTime);
    void draw();
};