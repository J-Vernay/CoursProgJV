#include "dino_player.h"
#include <dino/dino_terrain.h>

#include <algorithm>
#include "Physics.h"
#include "Rendering.h"
#include "dino/RopeManager.h"
#include "CodeBase/dino_geometry.h"


void DinoObject::Init()
{
    Physics::colliders.push_back(this);
    Rendering::spriteRenderer.push_back(this);
    collider_radius = 8;
}

void DinoObject::Update(double timeSinceStart,float deltaTime, const DinoGamepad& gamepad)
{
    HandleInput(deltaTime,gamepad);
    AnimTimerUpdate(deltaTime);
    GenerateVertexBuffer();

    //======================

    dinoLasso.GenerateRopePoints(dinoPos);
    dinoLasso.ManageRopePointsTime(deltaTime);
    dinoLasso.ScanRopeIntersection();
    
    DrawRopePoints();
}

//================================================================================================

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

void DinoObject::TakeDamage()
{
    damageTimer = damageDuration;
    curentDinoState = damage;
    animFrame = 0;
    animTimer = 0.0f;
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
    if (vbufID_dinoTexture != 0) XDino_DestroyVertexBuffer(vbufID_dinoTexture);
    
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
    
    
}

void DinoObject::Draw()
{
    DinoVec2 spritePos = {dinoPos.x, dinoPos.y - 16};
    XDino_Draw(vbufID_dinoTexture, texID_dinoTexture, spritePos, 16);
}



void DinoObject::DinoShut()
{
    
}

//============================================================================================================

#include "dino/CodeBase/dino_draw_utils.h"


void DinoObject::DinoLasso::GenerateRopePoints(DinoVec2 dinoPos)
{
    ropePoints.push_back(PointInfo{});
    ropePoints[ropePoints.size()-1].pointPos = dinoPos;
}

void DinoObject::DinoLasso::ManageRopePointsTime(float deltaTime)
{

    for (int i = 0; i < ropePoints.size(); ++i) {
        ropePoints[i].timeAliveRemaining -= deltaTime;
    }

    if (ropePoints[0].timeAliveRemaining <= 0.0f) {
        ropePoints.erase(ropePoints.begin());
    }
}

void DinoObject::DinoLasso::ScanRopeIntersection()
{
    int count = ropePoints.size();

    if (count < 5)
        return;

    DinoVec2 C = ropePoints[count - 2].pointPos;
    DinoVec2 D = ropePoints[count - 1].pointPos;

    for (int i = 0; i < count - 3; ++i)  
    {
        DinoVec2 A = ropePoints[i].pointPos;
        DinoVec2 B = ropePoints[i + 1].pointPos;

        if (Dino_IntersectSegment(A, B, C, D))
        {
            loopLasso.assign(ropePoints.begin() + i + 1, ropePoints.end() - 1);
            ropePoints.erase(ropePoints.begin() + i + 1, ropePoints.end() - 1);
            break;
        }
    }
}

bool DinoObject::DinoLasso::WasInLoop(DinoVec2 pos)
{
    if (loopLasso.size() < 3)
        return false;

    DinoVec2 origin = {0, 0};

    int nbCollisions = 0;
    for (int i = 0; i < loopLasso.size(); ++i) {
        DinoVec2 c = loopLasso[i].pointPos;
        DinoVec2 d = (i == loopLasso.size() - 1) ? loopLasso[0].pointPos : loopLasso[i + 1].pointPos;
        if (Dino_IntersectSegment(origin, pos, c, d))
            nbCollisions += 1;
    }

    return (nbCollisions % 2) == 1;
}



void DinoObject::DrawRopePoints() 
{

    std::vector<DinoVec2> ropePointsPos = dinoLasso.GetRopePoint();
    

    if (RopeManager::allRopes.size() > playerId)RopeManager::allRopes[playerId] = ropePointsPos;
    else RopeManager::allRopes.push_back(ropePointsPos);

    std::vector<DinoVertex> line;

    Dino_GenVertices_Polyline(line,ropePointsPos, 3.0f,dinoLasso.color);

    uint64_t vbufID_dinoLine = XDino_CreateVertexBuffer(line.data(), line.size(), "dinoLine");
    
    XDino_Draw(vbufID_dinoLine, XDino_TEXID_WHITE);

    XDino_DestroyVertexBuffer(vbufID_dinoLine);
}























