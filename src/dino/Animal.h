#pragma once
#include "Collider.h"

#include <cstdint>
#include "dino/dino_draw_utils.h"
#include "dino/xdino.h"

class Animal : public Collider
{
    
    public:
        DinoVec2 Pos;
        DinoVec2 Destination;
        float distanceThreshold = 0.5f;
        float Speed = 50;
        int Id;
        void Init(uint64_t);
        void Update(double);
        void Shut();
        static void InitStatic(uint64_t);
        static void ShutStatic();


    DinoVec2 GetPosition() override
    {
        return Pos;
    }

    void SetPosition(DinoVec2 newPos) override
    {
        Pos = newPos;
    }

    private:
        
        int animFrame = 0;
        float animTimer = 0.f;
        int animPos = 0;

    
    


    static  uint64_t texID_Texture;


    bool isFadeInDone = false;
    float timeSinceSpawn = 1.0f;
     std::vector<DinoVertex> FadeIn(float, std::vector<DinoVertex>);

    
    DinoVec2 MoveTowards(DinoVec2,DinoVec2, float,float);
    DinoVec2 DefineDestination();
    void Draw();
    float Distance(DinoVec2,DinoVec2);
    EAnimalAnim ChooseAnim();


    uint64_t vbufID_Texture;

    
};
