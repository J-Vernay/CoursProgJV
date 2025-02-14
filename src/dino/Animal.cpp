#include "Animal.h"

#include "dino_draw_utils.h"

Animal::Animal()
{
    _id = 0;
    _dir = {0, 0};
    _active = false;
    _u = 0;
    _v = 0;
    _c = 0;
    _offset = 0;
    _animTimer = 0;
    inverted = false;
}

Animal::Animal(const std::string& tex_d, const uint16_t id, const DinoVec2 pos_d, const int radius_d, const uint16_t u, const uint16_t v) : Entity(tex_d, pos_d, radius_d)
{
    this->_id = id;
    _dir = {0, 0};
    _active = false;
    _u = u;
    _v = v;
    _c = 0;
    _offset = _u;
    _animTimer = 0;
    inverted = false;
}

void Animal::Update(const float deltaTime)
{
    pos.x += _dir.x * deltaTime * 50;
    pos.y += _dir.y * deltaTime * 50;

    _animTimer += deltaTime;

    inverted = _dir.x < 0;
    
    if (_animTimer >= 0.5f)
    {
        _animTimer = 0;
        _c = (_c + 1) % 8;
        _u = _offset + 32 * _c;
    }
}

void Animal::Draw()
{
    if (!_active)
        return;

    auto dc = inverted
                  ? Dino_CreateDrawCall_InvertedSprite(tex, 32, _u, _v, 2)
                  : Dino_CreateDrawCall_Sprite(tex, 32, _u, _v, 2);
    dc.translation = pos;
    XDino_Draw(dc);
}

void Animal::Spawn()
{
    _active = true;

    _dir = XDino_RandomUnitVec2();
    pos = XDino_RandomVec2_Between(480 - 16 * 15, 480 + 16 * 15, 360 - 12 * 12, 360 + 12 * 11);
}

bool Animal::GetActive() const
{
    return _active;
}

void Animal::SetActive(const bool active)
{
    _active = active;
}

void Animal::SetDir(const DinoVec2 dir)
{
    _dir = dir;
}
