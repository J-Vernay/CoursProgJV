#include "TileMap.h"

#include "AnimatedTile.h"

#include <iostream>

TileMap::TileMap()
{
    _width = 1;
    _height = 1;
    _scale = 1;
    _top_pos = {0,0};
}

TileMap::TileMap(const int scale, const int width, const int height)
{
    this->_width = width;
    this->_height = height;
    this->_scale = scale;
    constexpr float xo = 224;
    constexpr float yo = 192;
    _top_pos = {xo, yo};
}

void TileMap::Update(const float deltaTime) const
{
    for (auto const& tile : this->_tiles)
    {
        tile->Update(deltaTime);
    }
}

void TileMap::Draw() const
{
    for (auto const& tile : this->_tiles)
    {
        tile->Draw();
    }
    for (auto const& p : this->_props)
    {
        p->Draw();
    }
}

void TileMap::Init()
{
    const auto s = 32 * 32 + _width  * _height;
    _tiles.resize(s);
    
    int l = 0;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            const float px = static_cast<float>(16 * _scale * i);
            const float py = static_cast<float>(16 * _scale * j);
            const auto p = DinoVec2 {px, py};
            
            const auto w = new Tile("terrain.png", p, 0, 0);
            _tiles[l] = w;
            l++;
        }
    }
    
    const auto tr = DinoVec2 {_top_pos.x + static_cast<float>(16 * _scale * (_width - 1)), _top_pos.y};
    const auto bl = DinoVec2 {_top_pos.x, _top_pos.y + static_cast<float>(16 * _scale * (_height - 1))};
    const auto br = DinoVec2 {_top_pos.x + static_cast<float>(16 * _scale * (_width - 1)), _top_pos.y + static_cast<float>(16 * _scale * (_height - 1))};
    
    const auto a1 = new AnimatedTile("terrain.png", _top_pos, 0, 16);
    const auto a2 = new AnimatedTile("terrain.png", tr, 32, 16);
    const auto a3 = new AnimatedTile("terrain.png", bl, 0, 48);
    const auto a4 = new AnimatedTile("terrain.png", br, 32, 48);
    
    _tiles[l] = a1;
    _tiles[l + 1] = a2;
    _tiles[l + 2] = a3;
    _tiles[l + 3] = a4;
    l += 4;
    
    for (int i = 1; i < _width - 1; i++)
    {
        const auto cTopPos = DinoVec2 {_top_pos.x + static_cast<float>(16 * _scale * i), _top_pos.y};
        const auto cBotPos = DinoVec2 {_top_pos.x + static_cast<float>(16 * _scale * i), _top_pos.y + static_cast<float>(16 * _scale * (_height - 1))};
        
        const auto to = new AnimatedTile("terrain.png", cTopPos, 16, 16);
        const auto bo = new AnimatedTile("terrain.png", cBotPos, 16, 48);
        
        _tiles[l] = to;
        _tiles[l + 1] = bo;

        l += 2;
    }
    
    for (int i = 1; i < _height - 1; i++)
    {
        const auto cRightPos = DinoVec2 {_top_pos.x, _top_pos.y + static_cast<float>(16 * _scale * i)};
        const auto cLeftPos = DinoVec2 {_top_pos.x + static_cast<float>(16 * _scale * (_width - 1)), _top_pos.y + static_cast<float>(16 * _scale * i)};

        const auto ri = new AnimatedTile("terrain.png", cRightPos, 0, 32);
        const auto le = new AnimatedTile("terrain.png", cLeftPos, 32, 32);
        
        _tiles[l] = ri;
        _tiles[l + 1] = le;

        l += 2;
    }

    for (int i = 1; i < _width - 1; i++)
    {
        for (int j = 1; j < _height - 1; j++)
        {
            const auto cCenterPos = DinoVec2 {_top_pos.x + static_cast<float>(16 * _scale * i), _top_pos.y + static_cast<float>(16 * _scale * j)};

            const auto te = new Tile("terrain.png", cCenterPos, 16, 32);
            
            _tiles[l] = te;
            l++;
        }
    }

    const int r = XDino_RandomInt32(10, 20);
    _props.resize(r);

    for (int i = 0; i < r; i++)
    {
        const auto pp = XDino_RandomVec2_Between(230, static_cast<float>(230 + (_width - 2) * 16 * _scale), 200, static_cast<float>(200 + (_height - 2) * 16 * _scale));
        const auto v = XDino_RandomUint16(0, 2);

        auto p = new Tile("terrain.png", pp, 32 + 16 * v, 0);
        _props[i] = p;
    }
}
