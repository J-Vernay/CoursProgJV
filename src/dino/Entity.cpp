#include "Entity.h"

Entity::Entity()
{
    pos = {0,0};
}

Entity::Entity(const std::string& tex)
{
    this->tex = tex;
    pos = {0,0};
}

Entity::Entity(const std::string& tex, const DinoVec2 pos)
{
    this->tex = tex;
    this->pos = pos;
}
