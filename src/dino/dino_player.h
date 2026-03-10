#pragma once
#include <array>
#include "dino/xdino.h"




enum DinoState
{
    idle,
    walk,
    running,
    damage,
    DinoState_Count
};
struct AnimInfo
{
    int animPosStart;
    int animFrameNumber;
    float animRefresh;
};
static std::array<AnimInfo, DinoState_Count> animeStateInfo =
{
    AnimInfo{0,   4, 0.16f},   // idle
    AnimInfo{96,  6, 0.16f},//walk
    AnimInfo{432, 6, 0.16f},// run
    AnimInfo{336, 3, 0.08f},   // damage
        
};


struct DinoObject
{
    

    public:
        DinoVec2 dinoPos;
        uint64_t texID_dinoTexture;
        float damageTimer = 0.f;
        float dinoSpeed = 100;
        int playerId;
        void Update(double,float,DinoGamepad);
        void DinoShut(); 

    private:
    
        int animFrame = 0;
        float animTimer = 0.f;
        int animPos = 0;
    
        DinoState curentDinoState = idle;
        bool isGoingRigth;
        float damageDuration = 3.f;
    
        
        uint64_t vbufID_dinoTexture;
     
        void HandleInput(float,DinoGamepad);
        void AnimTimerUpdate(float);
        void GenerateVertexBuffer();
};

