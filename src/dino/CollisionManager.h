#pragma once
#include <vector>

#include "LassoPoint.h"
#include "Agent.h"
#include "Animal.h"
#include "PopUpScore.h"
#include "dino/xdino.h"

class DinoPlayer;

class CollisionManager
{
public:
    CollisionManager(GameManager& _gameManager);

    void Update(float deltaTime);
    
    void ApplyPlayersCollision(DinoArray<Agent*>& agentList);
    void ShuffleByVerticalPosition(DinoArray<Agent*>* agentList);
    void CheckLassoCollisionPlayer(DinoArray<DinoPlayer*>* pList);

    int CheckAddScore(const DinoArray<Agent*>& list);
    void ApplyCircleLasso(int playerWhoDo, const DinoArray<DinoVec2>& bounds);
    bool CheckIsInBounds(const DinoArray<DinoVec2>& bounds, DinoVec2 pos);
    
    void SetAgents(DinoArray<Agent*>* agents);
    void SetAnimals(DinoArray<Animal*>* animals);
    void SetPlayers(DinoArray<DinoPlayer*>* players);

    DinoArray<Agent*>* agentsList = nullptr;
    DinoArray<DinoPlayer*>* playerList = nullptr;
    DinoArray<Animal*>* animalList = nullptr;

    std::vector<PopUpScore*> popUpScoreList;

    GameManager& gameManager;
};
