#include "dino_animals.h"


// Constantes.
constexpr float ANIMAL_SPEED = 50.f; // Nombre de pixels parcourus en une seconde.

void DinoAnimal::Update(float deltaTime)
{
    up_ = false;
    left_ = false;
    down_ = false;

    pos_ = {pos_.x + dir_.x * ANIMAL_SPEED * deltaTime, pos_.y + dir_.y * ANIMAL_SPEED * deltaTime};
    
    mirror_ = dir_.x < 0;
    
    if (dir_.y > .5f || dir_.y < -.5f)
    {
        if (dir_.y > 0)
            up_ = true;
        if (dir_.y < 0)
            down_ = true;
    }
    else left_ = true;
    
}

void DinoAnimal::Display(double timeSinceStart)
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

        int rot = 0;
        if (left_)
            rot = 0;
        if (up_)
            rot = 32;
        if (down_)
            rot = 64;
        int pos = 0;
        pos += sprite_idx_ % 4 * 32;
        
        if (mirror_)
        {
            drawCall.vertices.emplace_back(posA, pos + animal_type_ * 128, rot);
            drawCall.vertices.emplace_back(posB, pos + animal_type_ * 128 + 32, rot);
            drawCall.vertices.emplace_back(posC, pos + animal_type_ * 128, rot + 32);
            drawCall.vertices.emplace_back(posB, pos + animal_type_ * 128 + 32, rot);
            drawCall.vertices.emplace_back(posC, pos + animal_type_ * 128, rot + 32);
            drawCall.vertices.emplace_back(posD, pos + animal_type_ * 128 + 32, rot + 32);
        }
        else
        {
            drawCall.vertices.emplace_back(posA, pos + animal_type_ * 128 + 32, rot);
            drawCall.vertices.emplace_back(posB, pos + animal_type_ * 128, rot);
            drawCall.vertices.emplace_back(posC, pos + animal_type_ * 128 + 32, rot + 32);
            drawCall.vertices.emplace_back(posB, pos + animal_type_ * 128, rot);
            drawCall.vertices.emplace_back(posC, pos + animal_type_ * 128 + 32, rot + 32);
            drawCall.vertices.emplace_back(posD, pos + animal_type_ * 128, rot + 32);
        }
        
        DinoVec2 offset = {16, 32};
        DinoVec2 newPos = {pos_.x - offset.x, pos_.y - offset.y};
        drawCall.translation = newPos;
        XDino_Draw(drawCall);
    }
}

void DinoAnimal::DinoAnimalInit(DinoVec2 pos, int idx)
{
    pos_ = pos;
    animal_type_ = idx;
    dir_ = XDino_RandomUnitVec2();
}

void DinoAnimal::OnTerrainBorder()
{
    dir_ = XDino_RandomUnitVec2();
}
