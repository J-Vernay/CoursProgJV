#pragma once

#include <dino/DinoArray.h>

class Tree;
class Terrain;
class DinoPlayer;

class GameManager
{    
private:
    float currentTime = 0;
    int gameTimer = 60;

    uint64_t textTimer;
    
    DinoArray<DinoPlayer*>* playerList = nullptr;

    DinoArray<int> playersScore;
    
    int numberPlayer = 0;

    
public:

    bool IsInGame = false;
    Terrain* terrain = nullptr;
    DinoArray<Tree*> treesList;

    std::vector<int> _currentplayerConnected;
    
    GameManager();
    void ShutDown();
    void Update(float deltaTime);

    void DrawLobby(float deltaTime);
    void DrawGame();

    void DrawTree();
    
    void StartGame();
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

    //Pause Menu
    bool isPaused = false;

    float inputCooldown = 0;
    
    enum PauseMenuOption
    {
        RESTART,
        RETURN_TO_LOBBY,
        ADJUST_TIMER,
        RESUME
    };
    
    int selectedOption = RESUME;
    
    void HandlePauseMenu();
    void DrawPauseMenu();
    void DrawPauseMenuOption(int optionIndex, float posY, std::vector<int> letters, bool isSelected);
    void RestartGame();
    void ReturnToLobby();
    void AdjustTimer(int seconds);

};
