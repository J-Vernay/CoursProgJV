#pragma once
#include "Component/Collider.h"
#include "Component/SpriteRenderer.h"

#include <array>
#include "dino/xdino.h"
#include "dino/Component/Collider.h"




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


class DinoObject  : public Collider,public SpriteRenderer
{

    public:
        DinoVec2 dinoPos;
        uint64_t texID_dinoTexture;
        float damageTimer = 0.f;
        float dinoSpeed = 100;
        int playerId;
        void Update(double,float, const DinoGamepad&);
        void DinoShut();
        void Init();

    DinoVec2 GetPosition()  override
    {
        return dinoPos;
    }

    void SetPosition(DinoVec2 newPos) override
    {
        dinoPos = newPos;
    }

    void ReactLoop(float = 0) override
    {
        TakeDamage();
    };

    private:
    
        int animFrame = 0;
        float animTimer = 0.f;
        int animPos = 0;
    
        DinoState curentDinoState = idle;
        bool isGoingRigth;
        float damageDuration = 3.f;
    
        
        uint64_t vbufID_dinoTexture;

     
        void HandleInput(float, const DinoGamepad&);
        void TakeDamage();
        void AnimTimerUpdate(float);
        void GenerateVertexBuffer();
        void Draw();

    //===================================================================
public:
    struct PointInfo
    {
        float timeAliveRemaining = 10.0f;

    public:
         DinoVec2 pointPos;
        
    };

public:
    struct DinoLasso {
        
        void GenerateRopePoints(DinoVec2 dinoPos);
        void ManageRopePointsTime(float);
        void ScanRopeIntersection();
        bool WasInLoop(DinoVec2 pos);

        std::vector<DinoVec2> GetRopePoint()
        {
            std::vector<DinoVec2> ropePointsPos;
            for (int i = 0; i < ropePoints.size(); ++i) {
                ropePointsPos.push_back(ropePoints[i].pointPos);
            }
            return ropePointsPos;
    
        }

        std::vector<PointInfo> ropePoints;
        
        std::vector<PointInfo> loopLasso;

        DinoColor color;
    };
    
public:
    
    void DrawRopePoints();
    DinoLasso dinoLasso;

    


    DinoObject()
    {
        Init();
    }
    
};

