#include "dino_player.h"
#include <dino/dino_terrain.h>

#include <algorithm>
#include "Physics.h"


void DinoObject::Init()
{
    Physics::colliders.push_back(this);
    collider_radius = 16;
}

void DinoObject::Update(double timeSinceStart,float deltaTime, const DinoGamepad& gamepad)
{
    HandleInput(deltaTime,gamepad);
    AnimTimerUpdate(deltaTime);
    GenerateVertexBuffer();
}



void DinoObject::HandleInput(float deltaTime, const DinoGamepad& gamepad)
{
    
    bool isRunning = gamepad.btn_right;
    float speedMultiplier = isRunning ? 3.0f : 1.0f;

    dinoPos.x += gamepad.stick_left_x *dinoSpeed * deltaTime * speedMultiplier;
    dinoPos.y += gamepad.stick_left_y * dinoSpeed * deltaTime* speedMultiplier;

    
    float minX = DinoTerrain::FenceStart.x;
    float maxX = DinoTerrain::FenceStart.x + DinoTerrain::Fence.x;

    float minY = DinoTerrain::FenceStart.y;
    float maxY = DinoTerrain::FenceStart.y + DinoTerrain::Fence.y;

    dinoPos.x = std::max(dinoPos.x, minX);
    dinoPos.x = std::min(dinoPos.x, maxX);

    dinoPos.y = std::max(dinoPos.y, minY);
    dinoPos.y = std::min(dinoPos.y, maxY);

    bool isInputLeft  = gamepad.dpad_left;
    bool isInputRight = gamepad.dpad_right;
    bool hasInput     = isInputLeft || isInputRight;

    if (gamepad.btn_left)
        damageTimer = damageDuration;

    if (damageTimer <= 0.0f)
    {
        if (hasInput)
        {
            isGoingRigth    = isInputRight;
            curentDinoState = isRunning ? running : walk;
        }
        else
        {
            curentDinoState = idle;
        }
    }
}


void DinoObject::AnimTimerUpdate(float deltaTime)
{
    AnimInfo anim ;
    if (damageTimer > 0.f)
    {
        damageTimer-= deltaTime;

        anim = animeStateInfo[damage];
        
    }
    else anim = animeStateInfo[curentDinoState];
    
    animTimer += deltaTime;

    if (animTimer >= anim.animRefresh)
    {
        animTimer = 0.f;
        animFrame++;
        if (animFrame >= anim.animFrameNumber)
            animFrame = 0;
    }

    animPos = anim.animPosStart + animFrame * 24;
}


void DinoObject::GenerateVertexBuffer()
{
    
    float u0 = animPos;
    float u1 = animPos + 24;

    float v0 = 24 * playerId;

    if (!isGoingRigth)
        std::swap(u0, u1);

    // LeftORigthHandler
    std::vector<DinoVertex> dinoVS;
    dinoVS.resize(6);

    dinoVS[0].pos = {-1,-1};
    dinoVS[1].pos = { 1,-1};
    dinoVS[2].pos = {-1, 1};
    dinoVS[3].pos = { 1,-1};
    dinoVS[4].pos = {-1, 1};
    dinoVS[5].pos = { 1, 1};

    for (int i=0;i<6;i++)
        dinoVS[i].color = DinoColor_WHITE;
    
    //TOP triangle ----------------------
    
    dinoVS[0].u = u0;
    dinoVS[0].v = v0;
    dinoVS[1].u = u1;
    dinoVS[1].v = v0;
    dinoVS[2].u = u0;
    dinoVS[2].v = v0+24;
    
    //--------------------------------
    
    dinoVS[3].u = u1;
    dinoVS[3].v = v0;
    dinoVS[4].u = u0;
    dinoVS[4].v = v0+24;
    dinoVS[5].u = u1;
    dinoVS[5].v = v0+24;
    
    //BOT triangle ----------------------
     vbufID_dinoTexture = XDino_CreateVertexBuffer(dinoVS.data(), dinoVS.size(), "dinoTexture");
     XDino_Draw(vbufID_dinoTexture, texID_dinoTexture, dinoPos, 24);
    XDino_DestroyVertexBuffer(vbufID_dinoTexture);
    
}


void DinoObject::DinoShut()
{
    
}

