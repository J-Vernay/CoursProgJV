#include "AnimatedTile.h"

#include <iostream>

AnimatedTile::AnimatedTile()
{
    tex = "terrain.png";
    pos = {0,0};

    i = 0;
    j = 0;
    animTimer = 0;
    offset = 0;
}

AnimatedTile::AnimatedTile(const std::string& tex_d, const DinoVec2 pos_d, const uint16_t u, const uint16_t v) : Tile(tex_d, pos_d, u, v)
{
    this->tex = tex_d;
    this->pos = pos_d;
    this->i = u;
    this->j = v;
    animTimer = 0;
    offset = v;
}

void AnimatedTile::Update(const float deltaTime)
{
    animTimer += deltaTime;

    if (animTimer > 0.5f)
    {
        animTimer = 0;
        k = (k + 1) % 4;
        j = offset + k * 48;
    }
}

