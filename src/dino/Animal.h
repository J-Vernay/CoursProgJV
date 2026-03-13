#pragma once
#include "Component/Collider.h"
#include "Component/SpriteRenderer.h"

#include <cstdint>
#include "dino/CodeBase/dino_draw_utils.h"
#include "dino/xdino.h"

class Animal : public Collider, public SpriteRenderer
{
    
    public:
        DinoVec2 Pos;
        DinoVec2 Destination;
        float distanceThreshold = 0.5f;
        float Speed = 50;
        int Id;

        bool isAlive = true;
        bool pointAdded; 
        bool fadeOutDone= false;
    
        void Init();
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


    void ReactLoop(float = 0) override
    {
        isAlive = false;
    };

    private:
        
        int animFrame = 0;
        float animTimer = 0.f;
        int animPos = 0;
         float fadeOutTimer = 1;

    
    


    static  uint64_t texID_Texture;


    bool isFadeInDone = false;
    float timeSinceSpawn = 1.0f;
     std::vector<DinoVertex> FadeIn(float, std::vector<DinoVertex>);
        std::vector<DinoVertex> FadeOut(float deltaTime, std::vector<DinoVertex> vs);


        DinoVec2 MoveTowards(DinoVec2,DinoVec2, float,float);
    DinoVec2 DefineDestination();
    void Draw() override;
    float Distance(DinoVec2,DinoVec2);
    EAnimalAnim ChooseAnim();


    uint64_t vbufID_Texture;

    
};
