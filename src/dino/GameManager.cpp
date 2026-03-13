#include "xdino.h"

#include <iostream>
#include <dino/GameManager.h>

GameManager::GameManager()
{
}

void GameManager::ShutDown()
{
    XDino_DestroyGpuTexture(textTimer);
    playersScore.Clear();
}

void GameManager::StartGame(int _numberPlayer)
{
    currentTime = gameTimer;
    textTimer = XDino_CreateGpuTexture("monogram-bitmap.png");
    numberPlayer = _numberPlayer;

    for (int i = 0; i < numberPlayer; i++) {
        playersScore.AddBack(0);
    }
}

void GameManager::UpdateGame(float deltaTime)
{
    if (!isPaused)
        currentTime -= deltaTime;
    
    DrawTimer();
    DrawScores();
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
    
    uint64_t buffTimerDizaine = DrawNumber(d);
    uint64_t buffTimerUnite = DrawNumber(u);

    XDino_Draw(buffTimerDizaine, textTimer, posDizaire);
    XDino_Draw(buffTimerUnite, textTimer, posUnite);

    XDino_DestroyVertexBuffer(buffTimerUnite);
    XDino_DestroyVertexBuffer(buffTimerDizaine);
}

void GameManager::DrawScores()
{
    DinoVec2 render = XDino_GetRenderSize();    

    for (int i = 0; i < numberPlayer; ++i)
    {
        float posX = 64 + i*96;
        float posY = render.y - 32;
        
        uint64_t buffBackground = DrawSquareBackGround();
        XDino_Draw(buffBackground, textTimer, {posX + 16,posY + 8});
        
        DinoArray<uint64_t> buffScores;
        buffScores.AddBack(DrawLetter(19, i));
        buffScores.AddBack(DrawLetter(3, i));
        buffScores.AddBack(DrawLetter(15, i));
        buffScores.AddBack(DrawLetter(18, i));
        buffScores.AddBack(DrawLetter(5, i));

        for (int j = 0; j < buffScores.GetSize(); ++j)
        {
            XDino_Draw(buffScores[j], textTimer, {posX + (j*8),posY});
        }
        
        int u = playersScore[i] % 10;
        int d = (playersScore[i] / 10) % 10;
        int c = (playersScore[i] / 100) % 10;
        int m = (playersScore[i] / 1000) % 10;
    
        uint64_t buffScoreDizaine = DrawNumber(d, i);
        uint64_t buffScoreUnite = DrawNumber(u, i);
        uint64_t buffScoreCentaine = DrawNumber(c, i);
        uint64_t buffScoreMiller = DrawNumber(m, i);

        XDino_Draw(buffScoreMiller, textTimer, {posX-4, posY + 16}, 0.5);
        XDino_Draw(buffScoreCentaine, textTimer, {posX + 8, posY + 16}, 0.5);
        XDino_Draw(buffScoreDizaine, textTimer, {posX + 20, posY + 16}, 0.5);
        XDino_Draw(buffScoreUnite, textTimer, {posX + 32, posY+ 16}, 0.5);

        XDino_DestroyVertexBuffer(buffBackground);

        for (uint64_t vbuf_id : buffScores)
            XDino_DestroyVertexBuffer(vbuf_id);
    }
}

#pragma region Drawing

uint64_t GameManager::DrawNumber(int number, int colorID)
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
        DinoColor c = GetColor(colorID);

        if (colorID == -1)
        {
            if (currentTime <= gameTimer/2)
            {
                c = DinoColor_YELLOW;    
            }
            if (currentTime <= gameTimer/6)
            {
                c = DinoColor_RED;
            }
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

uint64_t GameManager::DrawLetter(int letter, int colorID)
{
    uint64_t newBuff;
    {
        std::vector<DinoVertex> vs(6);

        float sizeX = 3;
        float sizeY = 6;
        
        vs[0].pos = {-sizeX, -sizeY};
        vs[1].pos = { sizeX, -sizeY};
        vs[2].pos = {-sizeX,  sizeY};

        vs[3].pos = { sizeX, -sizeY};
        vs[4].pos = {-sizeX,  sizeY};
        vs[5].pos = { sizeX,  sizeY};

        DinoColor c = GetColor(colorID);

        for (int i = 0; i < 6; i++) {
            vs[i].color = c;
        }

        uint16_t offsetU;
        uint16_t offsetV = 0;

        if (letter < 16)
        {
            offsetU = 6 * letter;
        }
        else
        {
            offsetV = 12;
            offsetU = 6 * (letter - 16);
        }
        
        vs[0].u = 0 + offsetU; vs[0].v = 48 + offsetV;
        vs[1].u = 6 + offsetU; vs[1].v = 48 + offsetV;
        vs[2].u = 0 + offsetU; vs[2].v = 58 + offsetV;
        vs[3].u = 6 + offsetU; vs[3].v = 48 + offsetV;
        vs[4].u = 0 + offsetU; vs[4].v = 58 + offsetV;
        vs[5].u = 6 + offsetU; vs[5].v = 58 + offsetV;

        newBuff = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Score");
    }

    return newBuff;
}

uint64_t GameManager::DrawSquareBackGround(int colorID)
{
    uint64_t newBuff;
    {
        std::vector<DinoVertex> vs(6);

        float sizeX = 32;
        float sizeY = 16;
        
        vs[0].pos = {-sizeX, -sizeY};
        vs[1].pos = { sizeX, -sizeY};
        vs[2].pos = {-sizeX,  sizeY};

        vs[3].pos = { sizeX, -sizeY};
        vs[4].pos = {-sizeX,  sizeY};
        vs[5].pos = { sizeX,  sizeY};

        DinoColor c = GetColor(colorID);
        c.a = 0x55;
        
        for (int i = 0; i < 6; i++)
            vs[i].color = c;

        vs[0].u = 0 ; vs[0].v = 0;
        vs[1].u = 24 ; vs[1].v = 0;
        vs[2].u = 0 ; vs[2].v = 24;
        vs[3].u = 24 ; vs[3].v = 0;
        vs[4].u = 0 ; vs[4].v = 24;
        vs[5].u = 24 ; vs[5].v = 24;

        newBuff = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Score");
    }

    return newBuff;
}

uint64_t GameManager::GetTextureFont()
{
    return textTimer;
}

DinoColor GameManager::GetColor(int colorID)
{
    switch (colorID)
    {
    case -1 :
        return DinoColor_BLACK;
    case 0 :
        return DinoColor_BLUE;
    case 1 :
        return DinoColor_RED;
    case 2 :
        return DinoColor_YELLOW;
    case 3 :
        return DinoColor_GREEN;
    case 4 :
        return DinoColor_DARK_BLUE;
    case 5 :
        return DinoColor_DARK_RED;
    case 6 :
        return DinoColor_DARK_YELLOW;
    case 7 :
        return DinoColor_DARK_GREEN;
    default : return DinoColor_WHITE;
    }
}

#pragma endregion comment

float GameManager::GetCurrentT_Time()
{
    return currentTime / gameTimer;
}

bool GameManager::IsPaused()
{
    return isPaused;
}

void GameManager::AddScore(int playerId, int score)
{
    playersScore[playerId] += score;
}
