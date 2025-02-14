#include "Tile.h"

#include "Entity.h"
#include "dino_draw_utils.h"

Tile::Tile()
{
    tex = "terrain.png";
    pos = {0,0};

    i = 0;
    j = 0;
}

Tile::Tile(const std::string& tex_d) : Entity(tex_d)
{
    this->tex = tex_d;
    pos = {0,0};

    i = 0;
    j = 0;
}

Tile::Tile(const std::string& tex_d, const DinoVec2 pos_d) : Entity(tex_d, pos_d, 0)
{
    this->tex = tex_d;
    this->pos = pos_d;
    
    i = 0;
    j = 0;
}

Tile::Tile(const std::string& tex_d, const DinoVec2 pos_d, const uint16_t u, const uint16_t v) : Entity(tex_d, pos_d, 0)
{
    this->tex = tex_d;
    this->pos = pos_d;
    this->i = u;
    this->j = v;
}

void Tile::Update(const float deltaTime)
{
    
}

void Tile::Draw()
{
    auto dc = Dino_CreateDrawCall_Sprite(tex, 16, i, j, 2);
    dc.translation = pos;
    XDino_Draw(dc);
}
