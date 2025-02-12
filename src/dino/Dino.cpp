#include "Dino.h"

#include <iostream>
#include <ostream>

#pragma region Constructors

Dino::Dino()
{
    tex = "dinosaurs.png";
    pos = {0, 0};
    id = 0;
    animationTime = 0;
    i = 0;
    j = 24;
    k = 0;
    f = 0;
    stunned = false;
    inverted = false;
}

Dino::Dino(const std::string& tex, const uint16_t id)
{
    this->tex = tex;
    this->pos = {0, 0};
    this->id = id;
    animationTime = 0;
    i = 0;
    j = id * 24;
    k = 0;
    f = 0;
    stunned = false;
    inverted = false;
}

Dino::Dino(const std::string& tex, const uint16_t id, const DinoVec2 pos)
{
    this->tex = tex;
    this->pos = pos;
    this->id = id;
    animationTime = 0;
    i = 0;
    j = id * 24;
    k = 0;
    f = 0;
    stunned = false;
    inverted = false;
}

#pragma endregion

#pragma region Methods

bool Dino::CompareHeight(const Dino& d1, const Dino& d2)
{
    return d1.pos.y < d2.pos.y;
}

void Dino::Update(const float deltaTime, const DinoGamepad gamepad)
{
    float s = 1;
    uint16_t o;
    
    if (!stunned)
    {
        if (gamepad.stick_left_x < -0.1)
            inverted = true;
        else if (gamepad.stick_left_x > 0.1)
            inverted = false;
        
        pos.x += gamepad.stick_left_x * 300 * deltaTime;
        pos.y += gamepad.stick_left_y * 300 * deltaTime;
        
        if (gamepad.btn_down && (abs(gamepad.stick_left_x) > 0.1 || abs(gamepad.stick_left_y) > 0.1))
        {
            pos.x += gamepad.stick_left_x * 300 * deltaTime;
            pos.y += gamepad.stick_left_y * 300 * deltaTime;
            s = 2;
            o = 432;
            f = 6;
        }
        else if (abs(gamepad.stick_left_x) > 0.1 || abs(gamepad.stick_left_y) > 0.1)
        {
            o = 96;
            f = 6;
        }
        else
        {
            o = 0;
            f = 4;
        }
    }
    else
    {
        o = 336;
        f = 3;
    }

    SetUV(deltaTime * s, o);
}

void Dino::Draw(DinoDrawCall* dc) const
{
    dc->translation = pos;
    
    dc->vertices[0].u = inverted ? 24 + i : i;
    dc->vertices[0].v = j;
    
    dc->vertices[1].u = inverted ? i : 24 + i;
    dc->vertices[1].v = j;
    
    dc->vertices[2].u = inverted ? 24 + i : i;
    dc->vertices[2].v = 24 + j;
    
    dc->vertices[3].u = inverted ? i : 24 + i;
    dc->vertices[3].v = j;
    
    dc->vertices[4].u = inverted ? 24 + i : i;
    dc->vertices[4].v = 24 + j;
    
    dc->vertices[5].u = inverted ? i : 24 + i;
    dc->vertices[5].v = 24 + j;
    
    dc->textureName = tex;
}

void Dino::SetUV(const float deltaTime, const uint16_t offset)
{
    animationTime += deltaTime;

    if (animationTime > 0.125)
    {
        animationTime = 0;
        k = (k + 1) % f;
        
        i = k * 24 + offset;
        j = 24 * id;
    }
}

#pragma endregion
