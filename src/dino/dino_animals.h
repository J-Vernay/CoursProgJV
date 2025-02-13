#pragma once

#include <dino/xdino.h>
#include "dino_player.h"

class DinoAnimal
{
    
public:
    void UpdateAnimal(float deltaTime);
    void DisplayAnimal(double timeSinceStart);
    void DinoAnimalInit(DinoVec2 pos, int idx);
    bool IsAbove(DinoPlayer& other);
    
private:
    DinoVec2 pos_ = {};
    DinoVec2 dir_ = {};
    bool mirror_ = false;

    bool up_ = false;
    bool left_ = false;
    bool down_ = false;

    int sprite_idx_ = 0;
    int animal_type_ = 0;
};
