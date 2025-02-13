#include "dino_animals.h"


// Constantes.
constexpr float ANIMAL_SPEED = 50.f; // Nombre de pixels parcourus en une seconde.

void DinoAnimal::UpdateAnimal(float deltaTime)
{
    up_ = false;
    left_ = false;
    down_ = false;

    pos_ = {pos_.x + dir_.x * ANIMAL_SPEED * deltaTime, pos_.y + dir_.y * ANIMAL_SPEED * deltaTime};
    
}

void DinoAnimal::DisplayAnimal(double timeSinceStart)
{
    //Afficher le dinosaure
    {
        sprite_idx_ = static_cast<int>(timeSinceStart / 0.12);
        
        DinoDrawCall drawCall;
        drawCall.textureName = "animals.png";
        DinoVec2 posA = {0,0};
        DinoVec2 posB = {32, 0};
        DinoVec2 posC = {0,32};
        DinoVec2 posD = {32,32};

        int pos = 0;
        if (up_)
            pos = 32;
        if (left_)
            pos = 0;
        if (down_)
            pos = 64;
        pos += sprite_idx_ % 4 * 32;
        
        if (mirror_)
        {
            drawCall.vertices.emplace_back(posA, pos + 32, animal_type_ * 32);
            drawCall.vertices.emplace_back(posB, pos, animal_type_ * 32);
            drawCall.vertices.emplace_back(posC, pos + 32, 32 + animal_type_ * 32);
            drawCall.vertices.emplace_back(posB, pos, animal_type_ * 32);
            drawCall.vertices.emplace_back(posC, pos + 32, 32 + animal_type_ * 32);
            drawCall.vertices.emplace_back(posD, pos, 32 + animal_type_ * 32);
        }
        else
        {
            drawCall.vertices.emplace_back(posA, pos, animal_type_ * 32);
            drawCall.vertices.emplace_back(posB, pos + 32, animal_type_ * 32);
            drawCall.vertices.emplace_back(posC, pos, 32 + animal_type_ * 32);
            drawCall.vertices.emplace_back(posB, pos + 32, animal_type_ * 32);
            drawCall.vertices.emplace_back(posC, pos, 32 + animal_type_ * 32);
            drawCall.vertices.emplace_back(posD, pos + 32, 32 + animal_type_ * 32);
        }
        
        drawCall.translation = pos_;
        XDino_Draw(drawCall);
    }
}

void DinoAnimal::DinoAnimalInit(DinoVec2 pos, int idx)
{
    pos_ = pos;
    animal_type_ = idx;
    dir_ = XDino_RandomUnitVec2();

}
