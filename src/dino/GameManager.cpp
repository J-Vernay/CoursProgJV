#include "DinoArray.h"
#include "xdino.h"

#include <dino/GameManager.h>

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
    
}

void GameManager::StartGame()
{
    currentTime = gameTimer;
    textTimer = XDino_CreateGpuTexture("monogram-bitmap.png");
}

void GameManager::UpdateGame(float deltaTime)
{
    if (!isPaused)
        currentTime -= deltaTime;
    
    DrawTimer();
}

void GameManager::SetTimerState()
{
    isPaused = !isPaused;
}

void GameManager::SetPlayers(DinoArray<DinoPlayer*>* players)
{
    playerList = players;
}

void GameManager::DrawTimer()
{
    DinoVec2 render = XDino_GetRenderSize();
    DinoVec2 posDizaire = {render.x /2 - 12, 36};
    DinoVec2 posUnite = {render.x /2 + 12, 36};
    
    int u = (int)currentTime % 10;
    int d = ((int)currentTime / 10) % 10;
    
    buffTimerDizaine = DrawNumber(d);
    buffTimerUnite = DrawNumber(u);

    XDino_Draw(buffTimerDizaine, textTimer, posDizaire);
    XDino_Draw(buffTimerUnite, textTimer, posUnite);
}

uint64_t GameManager::DrawNumber(int number)
{
    uint64_t newBuff;
    {
        std::vector<DinoVertex> vs(6);
        
        float sizeX = 12;
        float sizey = 24;
        
        vs[0].pos = {-sizeX, -sizey};
        vs[1].pos = {sizeX, -sizey};
        vs[2].pos = {-sizeX, sizey};
        
        vs[3].pos = {sizeX, -sizey};
        vs[4].pos = {-sizeX, sizey};
        vs[5].pos = {sizeX, sizey};
        
        //UV
        DinoColor c = DinoColor_BLACK;

        if (currentTime <= gameTimer/2)
        {
            c = DinoColor_YELLOW;    
        }
        if (currentTime <= gameTimer/6)
        {
            c = DinoColor_RED;    
        }
        
        vs[0].color = c;
        vs[1].color = c;
        vs[2].color = c;
        vs[3].color = c;
        vs[4].color = c;
        vs[5].color = c;

        uint16_t offset = 6*number;
        
        vs[0].u = 0 + offset;        vs[0].v = 36;

        vs[1].u = 6+ offset;        vs[1].v = 36;
        
        vs[2].u = 0+ offset;        vs[2].v = 48;

        vs[3].u = 6+ offset;        vs[3].v = 36;

        vs[4].u = 0+ offset;        vs[4].v = 48;

        vs[5].u = 6+ offset;        vs[5].v = 48;
        
        newBuff = XDino_CreateVertexBuffer(vs.data(), vs.size(), "LassoPoint");
    }

    return newBuff;
}

float GameManager::GetCurrentT_Time()
{
    return currentTime / gameTimer;
}

bool GameManager::IsPaused()
{
    return isPaused;
}
