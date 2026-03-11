#pragma once
#include "../../build/Agent.h"
#include "../../build/LassoPoint.h"
#include <dino/xdino.h>

class CollisionManager;

class DinoPlayer : public Agent
{    
public:
    DinoPlayer(float _speed, int idPlayer, DinoVec2 miniBound, DinoVec2 maxiBound, CollisionManager& _collisionManager);

    
    void Start();
    void Update(float deltaTime) override;
    void Move(float deltaTime);
    void ApplyAnimations(float deltaTime);
    void SetAnimationValues(short animStart, short numberFrame, short Ips);
    void ShutDown();
    void ApplyBounds();
    void TakeDamage() override;

    //Lasso
    void AddLassoPoint();
    void DrawLassos();
    void UpdateLasso(float deltaTime);
    uint64_t DrawLassoPoint();
    void RemoveOldLassoPoints();
    void CheckDoCircleLasso();
    void ResetLasso(int x);

    std::vector<LassoPoint> lassoPoints = {};
 
private:
    uint64_t texID_dino;
    uint64_t vbufID_dino;

    DinoVec2 lastPosition;
        
    short mirrorDino = 1;
    bool isMoving = false;
    bool isRunning = false;
    short offset = 0;
    short animFrame = 0;
    short currentAnimStart = 0;
    short numberSpriteCurrentAnim = 0;
    float IPS = 8;

    float baseSpeed = 10;

    DinoVec2 miniBound;
    DinoVec2 maxiBound;

    //Lasso
    uint64_t textureLasso;

    DinoColor lassoColor;
    
    float addLassoPointTimer = 0;
    float checkCircleTimer = 0;
    float TimePointLassoLived = 2;

    //Damaged
    bool isDamaged = false;
    float timeDamaged = 2;
    float elapsedTimeDamage = 0;
    float currentFrameAnim = 0;

    CollisionManager& collisionManager;
};