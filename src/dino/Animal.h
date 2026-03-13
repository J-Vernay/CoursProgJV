#pragma once
#include "dino/xdino.h"
#include <cmath>

#include "dino/DinoPlayer.h"

class Animal : public Agent
{
private:
    enum Direction
    {
        UP,DOWN,RIGHT,LEFT
    };

    uint64_t texID_animal;
    uint64_t vbufID_animal;

    DinoVec2 miniBound;
    DinoVec2 maxiBound;
    
    DinoVec2 nextPos = {};
    DinoVec2 lastPos = {};

    DinoVec2 windowSize;
    
    bool hasTarget = false;

    short isMirror = 1;
    int vBase = 0;
    
    float speed = 2.5;

    int animalType;

    int currentAnimFrame = 0;
    float currentAnimTime = 0;

    int verticalMove;

    Direction direction;

    float alpha;
    float alphaSpeedSpawn = -1;
    float alpahSpeedDespawn = -1;

    float t = 0;
    
    void Spawn();
    void Despawn();
    void Draw();
    void ApplyAnimations(float deltaTime);
    void ApplyMovement(float deltaTime);
    void ApplyBounds();

    GameManager& gameManager;
    
        
public:
    
    Animal(int type, DinoVec2 max_X, DinoVec2 max_Y, GameManager& gameManager);
    void Update(float deltaTime) override;
    void Shutdown();

    void TakeDamage() override;
};
