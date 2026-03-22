#include "GameManager.h"
#include "Terrain.h"

#include <__msvc_ostream.hpp>
#include <iostream>
#include <dino/Tree.h>

Tree::Tree(int _saison, GameManager& _gameManager) : gameManager(_gameManager)
{
    _saisonTree = _saison;
    Draw();

    typeAgent = Arbre;
}

void Tree::Draw()
{
    texID_tree = XDino_CreateGpuTexture("terrain.png");
    
    {
        std::vector<DinoVertex> vs;
        vs.resize(6);

        float x = 48/2;
        float y = 72/2;
        
        vs[0].pos = {-x, -y};
        vs[1].pos = {x, -y};
        vs[2].pos = {-x, y};
        
        vs[3].pos = {x, -y};
        vs[4].pos = {-x, y};
        vs[5].pos = {x, y};

        uint16_t saison = 80 * _saisonTree;
            
        vs[0].u = 48 + saison;         vs[0].v = 16;
        vs[1].u = 80 +saison;        vs[1].v = 16;
        vs[2].u = 48 +saison;        vs[2].v = 64;
        vs[3].u = 80 +saison;        vs[3].v = 16;
        vs[4].u = 48 +saison;        vs[4].v = 64;
        vs[5].u = 80 +saison;        vs[5].v = 64;
        
        vbufID_tree = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Ocean");
    }
}

void Tree::Update(float deltaTime)
{
    Agent::Update(deltaTime);

    if (!gameManager.IsInGame)
    {
        XDino_Draw(vbufID_tree, texID_tree, position);
    }
}

void Tree::Shutdown()
{
    
}

void Tree::SetPosition(float x, float y)
{
    Agent::SetPosition(x, y);
}

DinoVec2 Tree::GetMaxBouced()
{
    return
    {position.x + 8,
        position.y + 32
        };
}

DinoVec2 Tree::GetMinBouced()
{
    return
    {position.x - 8,
    position.y
    };
}

void Tree::TakeDamage()
{
    if (gameManager.IsInGame)return;
    
    gameManager.IsInGame = true;
    
    gameManager.terrain->randomSaison = _saisonTree;
    
    gameManager.terrain->DrawTerrain();
    gameManager.StartGame();
}