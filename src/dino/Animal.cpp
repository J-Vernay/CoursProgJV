#include "Animal.h"

#include <algorithm>
#include <iostream>

Animal::Animal(int type, DinoVec2 max_X, DinoVec2 max_Y, GameManager& gameManager) : gameManager(gameManager)
{
    DinoVec2 renderSize = XDino_GetRenderSize();

    float terrainOffsetX = renderSize.x / 2.f - (23 * 16 / 2.f);
    float terrainOffsetY = renderSize.y / 2.f - (15 * 16 / 2.f);

    miniBound.x = terrainOffsetX + max_X.x;
    miniBound.y = terrainOffsetY + max_X.y;
    maxiBound.x = terrainOffsetX + max_Y.x;
    maxiBound.y = terrainOffsetY + max_Y.y;

    float x = XDino_RandomFloat(miniBound.x, maxiBound.x);
    float y = XDino_RandomFloat(miniBound.y, maxiBound.y);
    position = {x, y};
    
    texID_animal = XDino_CreateGpuTexture("animals.png");
    animalType = type;

    Spawn();
}

void Animal::Draw()
{
    {
        uint64_t vbufID_animal;
        
        DinoArray<DinoVertex> vs(6);
        
        vs[0].pos = {-16, -16};
        vs[1].pos = {16, -16};
        vs[2].pos = {-16, 16};
        
        vs[3].pos = {16, -16};
        vs[4].pos = {-16, 16};
        vs[5].pos = {16, 16};

        int type = animalType * 128;

        short uAnimMini = 0 + (currentAnimFrame * 32) + type;
        short uAnimMax = 32 + (currentAnimFrame * 32) + type;

        uint8_t a = static_cast<uint8_t>(alpha * 255.0f);
        DinoColor color = {0xFF, 0xFF, 0xFF, a};
        
        vs[0].color = color;
        vs[1].color = color;
        vs[2].color = color;
        vs[3].color = color;
        vs[4].color = color;
        vs[5].color = color;
        
        if (isMirror != 0)
        {
            vs[0].u = isMirror == 1 ? uAnimMini : uAnimMax;        vs[0].v = 0 + vBase;
            vs[1].u = isMirror == 1 ? uAnimMax : uAnimMini ;        vs[1].v = 0 + vBase;
            vs[2].u = isMirror == 1 ? uAnimMini : uAnimMax;        vs[2].v = 32 + vBase;
            vs[3].u = isMirror == 1 ? uAnimMax : uAnimMini;        vs[3].v = 0 + vBase;
            vs[4].u = isMirror == 1 ? uAnimMini : uAnimMax;        vs[4].v = 32 + vBase;
            vs[5].u = isMirror == 1 ? uAnimMax : uAnimMini;        vs[5].v = 32 + vBase;
        }else
        {
            vs[0].u = uAnimMini;        vs[0].v = 0  + vBase;
            vs[1].u = uAnimMax ;        vs[1].v = 0 + vBase;
            vs[2].u = uAnimMini ;        vs[2].v = 32 + vBase;
            vs[3].u = uAnimMax ;        vs[3].v = 0 + vBase;
            vs[4].u = uAnimMini ;        vs[4].v = 32 + vBase;
            vs[5].u = uAnimMax ;        vs[5].v = 32 + vBase;
        }
        
        vbufID_animal = XDino_CreateVertexBuffer(vs.data(), vs.GetSize(), "Ocean");
        XDino_Draw(vbufID_animal, texID_animal, position);
    }
}

void Animal::Update(float deltaTime)
{
    ApplyAnimations(deltaTime);
    ApplyMovement(deltaTime);
    ApplyBounds();
}

void Animal::ApplyBounds()
{
    if (position.x < miniBound.x)position.x = miniBound.x + 1;
    if (position.y < miniBound.y)position.y = miniBound.y + 1;
    if (position.x > maxiBound.x)position.x = maxiBound.x - 1;
    if (position.y > maxiBound.y)position.y = maxiBound.y - 1;
}

void Animal::TakeDamage()
{
    Despawn();
}

void Animal::ApplyAnimations(float deltaTime)
{
    Draw();

    if (alphaSpeedSpawn < 1.0f)
    {
        alphaSpeedSpawn += deltaTime;
        alphaSpeedSpawn = std::clamp(alphaSpeedSpawn, 0.0f, 1.0f);
        alpha = alphaSpeedSpawn;
    }

    if (alpahSpeedDespawn > 0)
    {
        alpahSpeedDespawn -= deltaTime;
        alpahSpeedDespawn = std::clamp(alpahSpeedDespawn, 0.0f, 1.0f);
        alpha = alpahSpeedDespawn;
        
        if (alpahSpeedDespawn <= 0.1f)
        {
            readyToDelete = true;
        }
    }


    currentAnimTime += deltaTime;

    if (currentAnimTime >= 0.15f)
    {
        currentAnimTime = 0;
        currentAnimFrame = (currentAnimFrame + 1) % 4;
    }
}

void Animal::ApplyMovement(float deltaTime)
{
    if (gameManager.IsPaused())return;
    
     if (!hasTarget)
     {
         nextPos.x = XDino_RandomFloat(miniBound.x, maxiBound.x);
         nextPos.y = XDino_RandomFloat(miniBound.y, maxiBound.y);
         hasTarget = true;
     }else
     {
         float distance = sqrt(
             pow(position.x - nextPos.x, 2) +
             pow(position.y - nextPos.y, 2)
         );
         
         if (distance > 5)
         {
             position.x = position.x + (nextPos.x - position.x) / distance * speed * 0.1f;
             position.y = position.y + (nextPos.y - position.y) / distance * speed * 0.1f;
         }else
         {
             hasTarget = false;
         }
     }
    
    float dx = nextPos.x - position.x;
    float dy = nextPos.y - position.y;

    if (abs(dx) < abs(dy))
    {
        if (dy > 0)
        {
            vBase = 32;
        }
        else
        {
            vBase = 64;
        }
        isMirror = 0;
    }
    else
    {
        vBase = 0;
        
        if (dx > 0)
        {
            isMirror = -1;
        }else
        {
            isMirror = 1;
        }
    }

    lastPos = position;
}

void Animal::Spawn()
{
    alphaSpeedSpawn = 0;
}

void Animal::Despawn()
{
    alpahSpeedDespawn = 1;
}
