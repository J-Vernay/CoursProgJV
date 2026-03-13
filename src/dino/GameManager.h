#pragma once

#include <dino/DinoArray.h>

class DinoPlayer;

class GameManager{    
private:
    float currentTime = 0;
    int gameTimer = 60;
    bool isPaused = false;

    uint64_t textTimer;
    
    DinoArray<DinoPlayer*>* playerList = nullptr;

    DinoArray<int> playersScore;

    int numberPlayer = 0;
    
public:
    GameManager();
    void ShutDown();
    void StartGame(int numberPlayer);
    void UpdateGame(float deltaTime);
    void DrawTimer();
    void DrawScores();
    uint64_t DrawNumber(int number, int colorID = -1);
    uint64_t DrawLetter(int letter, int colorID = -1);
    uint64_t DrawSquareBackGround(int colorID = -1);
    uint64_t GetTextureFont();
    DinoColor GetColor(int colorID);

    float GetCurrentT_Time();
    bool IsPaused();

    void SetTimerState();
    void SetPlayers(DinoArray<DinoPlayer*>* players);

    void AddScore(int playerId, int score);
};
