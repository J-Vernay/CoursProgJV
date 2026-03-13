#pragma once

#include "GameManager.h"

#include <dino/DinoArray.h>

class DinoPlayer;

class PopUpScore {
private:
    uint64_t buffPopUnite = 0;
    uint64_t buffPopDizaine = 0;
    uint64_t buffPopPlus = 0;
    uint64_t textTimer = 0;

    GameManager* gameManager;
    int score = 0;
    DinoVec2 pos;
    
    float elapsedTime = 1;
    
public:
    PopUpScore(DinoVec2 pos, int score, GameManager* _gameManager, int playerID);
    ~PopUpScore();

    void Update(float deltaTime);

    uint64_t DrawPlus(int colorID);
    
    bool readyToDelete = false;
};