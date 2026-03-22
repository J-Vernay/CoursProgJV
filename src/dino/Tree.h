#pragma once
#include "Agent.h"
#include "dino/xdino.h"

class GameManager;

class Tree : Agent {
public:
    Tree(int _saison, GameManager& _gameManager);
    void Draw();
    void Update(float deltaTime) override;
    void Shutdown();
    virtual void SetPosition(float x, float y);

    uint16_t _saisonTree = 0;

    DinoVec2 GetMaxBouced();
    DinoVec2 GetMinBouced();

    void TakeDamage() override;
    
private:
    uint64_t texID_tree;
    uint64_t vbufID_tree;
    GameManager& gameManager;
};