#include "Animal.h"
#include "Terrain.h"
#include "Tree.h"
#include "xdino.h"

#include <iostream>
#include <dino/GameManager.h>

GameManager::GameManager() : treesList(0, 4)
{
    for (size_t i = 0 ; i < 4; i++) {
        treesList.AddBack(new Tree(i, *this));
    }

    terrain = new Terrain();
    terrain->SetUpTerrain();
}

void GameManager::ShutDown()
{
    XDino_DestroyGpuTexture(textTimer);
    playersScore.Clear();
}

void GameManager::StartGame()
{
    currentTime = gameTimer;
    textTimer = XDino_CreateGpuTexture("monogram-bitmap.png");
    numberPlayer = _currentplayerConnected.size();

    for (int i = 0; i < numberPlayer; i++) {
        playersScore.AddBack(0);
    }
}

void GameManager::Update(float deltaTime)
{
    terrain->Update(deltaTime);
    
    if (IsInGame)
    {
        if (isPaused)
        {
            inputCooldown -= deltaTime;
            if (inputCooldown < 0) inputCooldown = 0;
            
            HandlePauseMenu();
        }
        else
        {
            currentTime -= deltaTime;
        }

        if (currentTime <= 0)
        {
            IsInGame = false;
            playersScore.Clear();
            numberPlayer = 0;
        }
        
        DrawGame();
        
        if (isPaused)
        {
            DrawPauseMenu();
        }
    }
    else
    {
        DrawLobby(deltaTime);
    }
}

void GameManager::DrawLobby(float deltaTime)
{
    DinoVec2 renderSize = XDino_GetRenderSize();
    
    for (size_t i=0 ; i < 4; i++) {

        DinoVec2 position = {
            96,
            renderSize.y / 2 
        };
        
        treesList[i]->SetPosition(position.x + i*80, position.y-64);
        treesList[i]->Update(deltaTime);
    }
}

void GameManager::DrawGame()
{
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
    if (!IsInGame || playerId >= playersScore.GetSize())
        return;
    
    playersScore[playerId] += score;
}

void GameManager::HandlePauseMenu()
{
    if (!isPaused) return;
    
    for (int i = 0; i < 4; i++)
    {
        DinoGamepad gamepad{};
        XDino_GetGamepad((DinoGamepadIdx)i, gamepad);
        
        if (inputCooldown <= 0)
        {
            if (gamepad.dpad_up)
            {
                selectedOption = (selectedOption - 1 + 4) % 4;
                inputCooldown = 0.2f;
            }
            else if (gamepad.dpad_down)
            {
                selectedOption = (selectedOption + 1) % 4;
                inputCooldown = 0.2f;
            }
            
            if (selectedOption == ADJUST_TIMER)
            {
                if (gamepad.dpad_left)
                {
                    AdjustTimer(-10);
                    inputCooldown = 0.2f;
                }
                else if (gamepad.dpad_right)
                {
                    AdjustTimer(10);
                    inputCooldown = 0.2f;
                }
            }
            
            if (gamepad.btn_right)
            {
                switch (selectedOption)
                {
                case RESTART:
                    RestartGame();
                    break;
                case RETURN_TO_LOBBY:
                    ReturnToLobby();
                    break;
                case RESUME:
                    isPaused = false;
                    break;
                case ADJUST_TIMER:
                    break;
                }
                inputCooldown = 0.2f;
            }
        }
    }
}

void GameManager::RestartGame()
{
    currentTime = gameTimer;
    
    for (int i = 0; i < playersScore.GetSize(); i++)
    {
        playersScore[i] = 0;
    }
    
    if (playerList != nullptr)
    {
        for (int i = 0; i < playerList->GetSize(); i++)
        {
            (*playerList)[i]->Start();
        }
    }
    
    isPaused = false;
}

void GameManager::ReturnToLobby()
{
    IsInGame = false;
    isPaused = false;
    
    playersScore.Clear();
    numberPlayer = 0;
}

void GameManager::AdjustTimer(int seconds)
{
    currentTime += seconds;
    
    if (currentTime < 0)
        currentTime = 0;
    if (currentTime > gameTimer)
        currentTime = gameTimer;
}

void GameManager::DrawPauseMenu()
{
    if (!isPaused) return;
    
    DinoVec2 render = XDino_GetRenderSize();
    
    {
        std::vector<DinoVertex> vs(6);
        
        vs[0].pos = {0, 0};
        vs[1].pos = {render.x, 0};
        vs[2].pos = {0, render.y};
        vs[3].pos = {render.x, 0};
        vs[4].pos = {0, render.y};
        vs[5].pos = {render.x, render.y};
        
        DinoColor darkOverlay = {0, 0, 0, 180};
        for (int i = 0; i < 6; i++)
            vs[i].color = darkOverlay;
        
        for (int i = 0; i < 6; i++)
        {
            vs[i].u = 0;
            vs[i].v = 0;
        }
        
        uint64_t overlayBuff = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Overlay");
        XDino_Draw(overlayBuff, textTimer, {}, 1);
        XDino_DestroyVertexBuffer(overlayBuff);
    }
    
    float startY = 80;
    
    if (selectedOption == RESTART)
        DrawPauseMenuOption(0, startY, {18, 5, 3, 15, 13, 13, 5, 14, 3, 5, 18}, true);
    else
        DrawPauseMenuOption(0, startY, {18, 5, 3, 15, 13, 13, 5, 14, 3, 5, 18}, false);
    
    if (selectedOption == RETURN_TO_LOBBY)
        DrawPauseMenuOption(1, startY + 40, {18, 5, 20, 15, 21, 18}, true);
    else
        DrawPauseMenuOption(1, startY + 40, {18, 5, 20, 15, 21, 18}, false);
    
    {
        DinoVec2 pos = {render.x / 2 - 50, startY + 80};
        int colorID = (selectedOption == ADJUST_TIMER) ? 2 : -1;
        
        uint64_t buffLess = DrawLetter(28, colorID);
        XDino_Draw(buffLess, textTimer, pos, 1);
        XDino_DestroyVertexBuffer(buffLess);
        pos.x += 12;
        
        int chronoLetters[] = {3, 8, 18, 15, 14, 15};
        for (int letter : chronoLetters)
        {
            uint64_t buff = DrawLetter(letter, colorID);
            XDino_Draw(buff, textTimer, pos, 1);
            XDino_DestroyVertexBuffer(buff);
            pos.x += 8;
        }
        
        pos.x += 8;
        
        int dizaines = ((int)currentTime / 10) % 10;
        int unites = (int)currentTime % 10;
        
        uint64_t buffDiz = DrawNumber(dizaines, colorID);
        XDino_Draw(buffDiz, textTimer, pos, 0.5f);
        XDino_DestroyVertexBuffer(buffDiz);
        pos.x += 12;
        
        uint64_t buffUnit = DrawNumber(unites, colorID);
        XDino_Draw(buffUnit, textTimer, pos, 0.5f);
        XDino_DestroyVertexBuffer(buffUnit);
        pos.x += 12;
        
        uint64_t buffS = DrawLetter(19, colorID);
        XDino_Draw(buffS, textTimer, pos, 1);
        XDino_DestroyVertexBuffer(buffS);
        pos.x += 12;
        
        uint64_t buffGreater = DrawLetter(29, colorID);
        XDino_Draw(buffGreater, textTimer, pos, 1);
        XDino_DestroyVertexBuffer(buffGreater);
    }
    
    if (selectedOption == RESUME)
        DrawPauseMenuOption(3, startY + 120, {18, 5, 16, 18, 5, 14, 4, 18, 5}, true);
    else
        DrawPauseMenuOption(3, startY + 120, {18, 5, 16, 18, 5, 14, 4, 18, 5}, false);
}

void GameManager::DrawPauseMenuOption(int optionIndex, float posY, std::vector<int> letters, bool isSelected)
{
    DinoVec2 render = XDino_GetRenderSize();
    
    float textWidth = letters.size() * 8;
    float posX = render.x / 2 - textWidth / 2;
    
    int colorID = isSelected ? 2 : -1;
    
    for (int letter : letters)
    {
        if (letter == 32)
        {
            posX += 8;
            continue;
        }
        
        uint64_t letterBuff = DrawLetter(letter, colorID);
        XDino_Draw(letterBuff, textTimer, {posX, posY}, 1);
        XDino_DestroyVertexBuffer(letterBuff);
        posX += 8;
    }
}