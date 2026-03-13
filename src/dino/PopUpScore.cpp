#include <iostream>
#include <dino/PopUpScore.h>

PopUpScore::PopUpScore(DinoVec2 _pos, int _score, GameManager* _gameManager, int playerID)
: gameManager(_gameManager), score(_score), pos(_pos)
{
    textTimer = gameManager->GetTextureFont();

    int u = score % 10;
    int d = (score / 10) % 10;
    
    int idColor = playerID + 4;
    
    buffPopPlus = DrawPlus(playerID+4);
    buffPopUnite = gameManager->DrawNumber(u, idColor);
    buffPopDizaine = gameManager->DrawNumber(d, idColor);
}

PopUpScore::~PopUpScore()
{
    if (buffPopUnite != 0)
        XDino_DestroyVertexBuffer(buffPopUnite);

    if (buffPopDizaine != 0)
        XDino_DestroyVertexBuffer(buffPopDizaine);
    
    if (buffPopPlus != 0)
        XDino_DestroyVertexBuffer(buffPopPlus);
}

void PopUpScore::Update(float deltaTime)
{
    if (readyToDelete)
        return;
    
    elapsedTime -= deltaTime;

    if (elapsedTime <= 0)
        readyToDelete = true;
    
    XDino_Draw(buffPopPlus, textTimer, {pos.x - 12, pos.y - (8 - (elapsedTime * 8))}, 0.5);
    XDino_Draw(buffPopDizaine, textTimer, {pos.x - 4, pos.y - (8 - (elapsedTime * 8))}, 0.4);
    XDino_Draw(buffPopUnite, textTimer, {pos.x + 4, pos.y - (8 - (elapsedTime * 8))}, 0.4);
}

uint64_t PopUpScore::DrawPlus(int colorID)
{
        uint64_t newBuff;
        {
            std::vector<DinoVertex> vs(6);
        
            float sizeX = 10;
            float sizey = 10;
        
            vs[0].pos = {-sizeX, -sizey};
            vs[1].pos = {sizeX, -sizey};
            vs[2].pos = {-sizeX, sizey};
        
            vs[3].pos = {sizeX, -sizey};
            vs[4].pos = {-sizeX, sizey};
            vs[5].pos = {sizeX, sizey};
        
            //UV
            DinoColor c = gameManager->GetColor(colorID);
        
            vs[0].color = c;
            vs[1].color = c;
            vs[2].color = c;
            vs[3].color = c;
            vs[4].color = c;
            vs[5].color = c;
            
            vs[0].u = 66;        vs[0].v = 26;

            vs[1].u = 72;        vs[1].v = 26;
        
            vs[2].u = 66;        vs[2].v = 32;

            vs[3].u = 72;        vs[3].v = 26;

            vs[4].u = 66;        vs[4].v = 32;

            vs[5].u = 72;        vs[5].v = 32;
        
            newBuff = XDino_CreateVertexBuffer(vs.data(), vs.size(), "LassoPoint");
        }

        return newBuff;
}