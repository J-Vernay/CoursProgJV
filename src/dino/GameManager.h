#pragma once
#include "DinoPlayer.h"

#include <iostream>

class GameManager {
private:
    float currentTime = 0;
    int gameTimer = 60;
    bool isPaused = false;

    uint64_t textTimer;
    uint64_t buffTimerDizaine;    
    uint64_t buffTimerUnite;

    DinoArray<DinoPlayer*>* playerList = nullptr;
    
public:
    GameManager();
    ~GameManager();
    void StartGame();
    void UpdateGame(float deltaTime);
    void DrawTimer();
    uint64_t DrawNumber(int number);

    float GetCurrentT_Time();
    bool IsPaused();

    void SetTimerState();
    void SetPlayers(DinoArray<DinoPlayer*>* players);
    
};
