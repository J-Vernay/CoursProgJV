#include "Entity.h"

Entity::Entity()
{
    pos = {0,0};
    radius = 0;
}

Entity::Entity(const std::string& tex)
{
    this->tex = tex;
    pos = {0,0};
    radius = 0;
}

Entity::Entity(const std::string& tex, const DinoVec2 pos, const int radius)
{
    this->tex = tex;
    this->pos = pos;
    this->radius = radius;
}

void Entity::Update(float detlaTime)
{
}

void Entity::Draw()
{
}
