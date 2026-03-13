#include "DinoPlayer.h"
#include "GameManager.h"

#include <cmath>
#include <iostream>
#include <dino/CollisionManager.h>

DinoPlayer::DinoPlayer(float _speed, int idPlayer, DinoVec2 _miniBound, DinoVec2 _maxiBound, CollisionManager& _collisionManager, GameManager& _gameManager)
    : lassoPoints(), collisionManager(_collisionManager), vbufID_dino(0), gameManager(_gameManager)
{
    DinoVec2 windowSize = XDino_GetRenderSize();

    baseSpeed = _speed;
    ID = idPlayer;

    float terrainOffsetX = windowSize.x / 2.f - (23 * 16 / 2.f);
    float terrainOffsetY = windowSize.y / 2.f - (15 * 16 / 2.f);

    miniBound.x = terrainOffsetX + _miniBound.x;
    miniBound.y = terrainOffsetY + _miniBound.y;
    maxiBound.x = terrainOffsetX + _maxiBound.x + 16;
    maxiBound.y = terrainOffsetY + _maxiBound.y + 16;
}

void DinoPlayer::Start()
{
    DinoVec2 windowSize = XDino_GetRenderSize();
    
    float terrainOffsetX = windowSize.x / 2.f - (23 * 16 / 2.f);
    float terrainOffsetY = windowSize.y / 2.f - (15 * 16 / 2.f);

    position = {
        (miniBound.x + maxiBound.x) / 2.f + ID * 24.f,
        (miniBound.y + maxiBound.y) / 2.f    };

    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");
    textureLasso = XDino_CreateGpuTexture("monogram-bitmap.png");
    
    switch (ID) {
        case 0: lassoColor = DinoColor_BLUE; break;
        case 1: lassoColor = DinoColor_RED; break;
        case 2: lassoColor = DinoColor_YELLOW; break;
        case 3: lassoColor = DinoColor_GREEN; break;
        default : lassoColor = DinoColor_WHITE; break;
    }

    lassoColor.a = 0xBB;

    typeAgent = Player;
}

void DinoPlayer::Update(float deltaTime)
{
    Agent::Update(deltaTime);
    
    Move(deltaTime);
    ApplyAnimations(deltaTime);
    ApplyBounds();
    
    XDino_Draw(vbufID_dino, texID_dino, position);
}


void DinoPlayer::Move(float deltaTime)
{    
    DinoGamepad gamepad{};
    XDino_GetGamepad((DinoGamepadIdx)ID, gamepad);
    
    float speed = baseSpeed;

    if (!isDamaged) {
        if (std::abs(gamepad.stick_left_x) > 0.1f)
            mirrorDino = gamepad.stick_left_x >= 0 ? 1 : -1;
    }
        
    isMoving = std::abs(gamepad.stick_left_x) > 0.1 || std::abs(gamepad.stick_left_y) > 0.1;
    
    //PauseGame
    if (gamepad.start) {
        gameManager.SetTimerState();
    }
    
    if (gameManager.IsPaused())
    {
        SetAnimationValues(0, 4, 8);
        return;
    }

    //Running
    if (gamepad.btn_right && isMoving)
    {
        speed = baseSpeed * 2;
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    
    if (elapsedTimeDamage > 0) {
        elapsedTimeDamage -= deltaTime;
        isDamaged = true;
    }else {
        isDamaged = false;
    }
        
    if (!isDamaged)
    {
        //Set Anim move
        if (isRunning) {
            SetAnimationValues(17, 6, 16);
        }else {
            if (isMoving) {
                SetAnimationValues(4, 6, 8);
            }
            else {
                SetAnimationValues(0, 4, 8);
            }
        }

        //Moving
        position.x += gamepad.stick_left_x * speed * deltaTime;
        position.y += gamepad.stick_left_y * speed * deltaTime;
    }else
    {
        SetAnimationValues(13, 4, 8);
    }
    
    lastPosition = position;
}


void DinoPlayer::SetAnimationValues(short animStart, short numberFrame, short Ips)
{
    currentAnimStart = animStart;
    numberSpriteCurrentAnim = numberFrame;
    IPS = Ips;
}

void DinoPlayer::ApplyAnimations(float deltaTime)
{
    {
        std::vector<DinoVertex> vs(6);

        DinoVec2 texSize = XDino_GetGpuTextureSize(texID_dino);
        
        vs[0].pos = {-12, -12};
        vs[1].pos = {12, -12};
        vs[2].pos = {-12, 12};
        
        vs[3].pos = {12, -12};
        vs[4].pos = {-12, 12};
        vs[5].pos = {12, 12};

        currentFrameAnim += deltaTime;

        if (currentFrameAnim >= (1/IPS))
        {
            animFrame = (animFrame + 1) % numberSpriteCurrentAnim;
            offset = currentAnimStart + animFrame;
            currentFrameAnim = 0;
        }
        
        short uAnimMini = 0 + (offset * 24);
        short uAnimMax = 24 + (offset * 24);

        short skin = ID * 24;

        //UV
        vs[0].u = mirrorDino == 1 ? uAnimMini : uAnimMax;        vs[0].v = skin;

        vs[1].u = mirrorDino == 1 ? uAnimMax : uAnimMini;        vs[1].v = skin;
        
        vs[2].u = mirrorDino == 1 ? uAnimMini : uAnimMax;        vs[2].v = 24 + skin;

        vs[3].u = mirrorDino == 1 ? uAnimMax : uAnimMini;        vs[3].v = skin;

        vs[4].u = mirrorDino == 1 ? uAnimMini : uAnimMax;        vs[4].v = 24 + skin;

        vs[5].u = mirrorDino == 1 ? uAnimMax : uAnimMini;        vs[5].v = 24 + skin;

        if (vbufID_dino != 0)
            XDino_DestroyVertexBuffer(vbufID_dino); 
        
        vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
    }
}


void DinoPlayer::ShutDown()
{
    if (vbufID_dino != 0) {
        XDino_DestroyVertexBuffer(vbufID_dino);
        vbufID_dino = 0;
    }

    if (texID_dino != 0) {
        XDino_DestroyGpuTexture(texID_dino);
        texID_dino = 0;
    }

    for (size_t i = 0; i < lassoPoints.GetSize(); i++) {
        if (lassoPoints[i].buff != 0) {
            XDino_DestroyVertexBuffer(lassoPoints[i].buff);
            lassoPoints[i].buff = 0;
        }
    }
    lassoPoints.Clear();

    if (textureLasso != 0) {
        XDino_DestroyGpuTexture(textureLasso);
        textureLasso = 0;
    }
}

void DinoPlayer::ApplyBounds()
{
    if (position.x < miniBound.x)position.x = miniBound.x + 1;
    if (position.y < miniBound.y)position.y = miniBound.y + 1;
    if (position.x > maxiBound.x)position.x = maxiBound.x - 1;
    if (position.y > maxiBound.y)position.y = maxiBound.y - 1;
}

void DinoPlayer::TakeDamage()
{
    if (isDamaged) return;

    std::cout << "Damaged" << std::endl;

    elapsedTimeDamage = timeDamaged;
}

//----------------------- LASSOS -----------------------

void DinoPlayer::UpdateLasso(float deltaTime)
{
    addLassoPointTimer += deltaTime;
    checkCircleTimer += deltaTime;

    for (size_t i = 0; i < lassoPoints.GetSize(); i++)
        lassoPoints[i].timeAlive += deltaTime;

    RemoveOldLassoPoints();
    
    AddLassoPoint();

    CheckDoCircleLasso();
    DrawLassos();
}


void DinoPlayer::DrawLassos()
{
    for (size_t i = 0 ; i < lassoPoints.GetSize() ; i++)
    {
        LassoPoint l = lassoPoints[i];
        XDino_Draw(l.buff, textureLasso, l.position);
    }
}

void DinoPlayer::RemoveOldLassoPoints()
{
    for (size_t i = 0; i < lassoPoints.GetSize(); )
    {
        if (lassoPoints[i].timeAlive >= 2.0f)
        {
            std::cout << "Removing lasso point, buff=" << lassoPoints[i].buff << std::endl;
            if (lassoPoints[i].buff != 0)
            {
                XDino_DestroyVertexBuffer(lassoPoints[i].buff);
                lassoPoints[i].buff = 0;
            }
            lassoPoints.RemoveAt(i);
        }
        else
        {
            i++;
        }
    }
}
void DinoPlayer::AddLassoPoint()
{
    if (!isMoving) return;
    
    uint64_t buff = DrawLassoPoint();

    DinoVec2 pos = {position.x + XDino_RandomFloat(-1, 1),
        position.y + XDino_RandomFloat(-1, 1)};
    
    LassoPoint point = {pos, buff, 0};
    lassoPoints.AddBack(point);
    
    std::cout << "After AddBack, size: " << lassoPoints.GetSize() 
              << " | last buff: " << lassoPoints[lassoPoints.GetSize()-1].buff << std::endl;
}

uint64_t DinoPlayer::DrawLassoPoint()
{
    uint64_t newBuff;
    {        
        DinoArray<DinoVertex> vs(6);
        
        float size = 2.5;
        
        vs[0].pos = {-size, -size};
        vs[1].pos = {size, -size};
        vs[2].pos = {-size, size};
        
        vs[3].pos = {size, -size};
        vs[4].pos = {-size, size};
        vs[5].pos = {size, size};

        //Color
        
        vs[0].color = lassoColor;
        vs[1].color = lassoColor;
        vs[2].color = lassoColor;
        vs[3].color = lassoColor;
        vs[4].color = lassoColor;
        vs[5].color = lassoColor;
        
        //UV
        
        vs[0].u = 72;        vs[0].v = 0;

        vs[1].u = 96;        vs[1].v = 0;
        
        vs[2].u = 72;        vs[2].v = 24;

        vs[3].u = 96;        vs[3].v = 0;

        vs[4].u = 72;        vs[4].v = 24;

        vs[5].u = 96;        vs[5].v = 24;
        
        newBuff = XDino_CreateVertexBuffer(vs.data(), vs.GetSize(), "LassoPoint");
    }

    return newBuff;
}


void DinoPlayer::CheckDoCircleLasso()
{
    for (size_t i = 0; i < lassoPoints.GetSize(); ++i)
    {
        for (size_t j = i + 1; j < lassoPoints.GetSize(); ++j)
        {
            if (j - i < 6) continue;

            float dx = lassoPoints[i].position.x - lassoPoints[j].position.x;
            float dy = lassoPoints[i].position.y - lassoPoints[j].position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 10.0f)
            {
                DinoArray<DinoVec2> points;
                for (size_t p = i; p <= j; p++)
                    points.AddBack(lassoPoints[p].position);

                if (points.GetSize() >= 3)
                    collisionManager.ApplyCircleLasso(ID, points);

                ResetLasso(i);
                return;
            }
        }
    }
}

void DinoPlayer::ResetLasso(int x)
{
    while (x < (int)lassoPoints.GetSize())
    {
        if (lassoPoints[x].buff != 0)
        {
            lassoPoints[x].Shutdown();
            lassoPoints[x].buff = 0;
        }
        lassoPoints.RemoveAt(x);
    }
}