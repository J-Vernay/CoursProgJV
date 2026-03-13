#include "CollisionManager.h"

#include "GameManager.h"
#include "PopUpScore.h"

#include <iostream>

#include "dino/DinoPlayer.h"
#include "dino/dino_geometry.h"

CollisionManager::CollisionManager(GameManager& _gameManager): gameManager(_gameManager)
{}


void CollisionManager::Update(float deltaTime)
{
    for (int i = (int)agentsList->GetSize() - 1; i >= 0; i--)
    {
        if ((*agentsList)[i]->readyToDelete)
        {
            Animal* animal = dynamic_cast<Animal*>((*agentsList)[i]);
            agentsList->RemoveAt(i);

            if (animal != nullptr)
            {
                for (int j = 0; j < (int)animalList->GetSize(); j++)
                {
                    if ((*animalList)[j] == animal)
                    {
                        animalList->RemoveAt(j);
                        break;
                    }
                }
            }
        }
    }

    for (int i = (int)popUpScoreList.size() - 1; i >= 0; i--)
    {
        popUpScoreList[i]->Update(deltaTime);

        if (popUpScoreList[i]->readyToDelete)
        {
            delete popUpScoreList[i];
            popUpScoreList.erase(popUpScoreList.begin() + i);
        }
    }
}

void CollisionManager::ApplyPlayersCollision(DinoArray<Agent*>& agentList)
{
    float radius = 8.0f;
    float minDist = radius * 1.5f;
    
    for (size_t i = 0; i < agentList.GetSize(); i++)
    {
        for (size_t j = i+1; j < agentList.GetSize(); j++)
        {
            DinoVec2 posA = agentList[i]->position;
            DinoVec2 posB = agentList[j]->position;

            float dx = posA.x - posB.x;
            float dy = posA.y - posB.y;

            float dist = dx * dx + dy * dy;

            if (dist < minDist * minDist && dist > 0.0001f)
            {
                float d = sqrtf(dist);

                float overlap = minDist - d;

                float nx = dx / d;
                float ny = dy / d;

                posA.x += nx * overlap * 0.5f;
                posA.y += ny * overlap * 0.5f;

                posB.x -= nx * overlap * 0.5f;
                posB.y -= ny * overlap * 0.5f;

                agentList[i]->position = posA;
                agentList[j]->position = posB;
            }
        }
    }
}

void CollisionManager::ShuffleByVerticalPosition(DinoArray<Agent*>* agentList)
{
    for (size_t i = 0; i < agentList->GetSize(); i++)
    {
        for (size_t j = i+1; j < agentList->GetSize(); j++)
        {
            if ((*agentList)[i]->position.y > (*agentList)[j]->position.y)
            {
                std::swap((*agentList)[i], (*agentList)[j]);
            }
        }
    }
}

void CollisionManager::CheckLassoCollisionPlayer(DinoArray<DinoPlayer*>* pList)
{
    for (size_t p = 0; p < pList->GetSize(); p++)
    {
        DinoPlayer* currentPlayer = (*pList)[p];

        for (size_t i = 0; i < currentPlayer->lassoPoints.GetSize(); ++i)
        {
            DinoVec2 lassoPos = currentPlayer->lassoPoints[i].position;

            for (size_t q = 0; q < pList->GetSize(); q++) 
            {
                if (q == p) continue;

                DinoPlayer* otherPlayer = (*pList)[q];

                float dx = lassoPos.x - otherPlayer->position.x;
                float dy = lassoPos.y - otherPlayer->position.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 2.0f)
                {
                    currentPlayer->ResetLasso(i);
                    break;
                }
            }
        }
    }
}


void CollisionManager::ApplyCircleLasso(int playerWhoDo, const DinoArray<DinoVec2>& bounds)
{
    DinoArray<Agent*> agentCatchs;
    Agent* currentPlayer = nullptr;
    
    for (int i = (int)agentsList->GetSize() - 1; i >= 0; i--)
    {
        if ((*agentsList)[i]->ID == playerWhoDo) {
            currentPlayer = (*agentsList)[i];
            continue;
        }
        
        Agent* currentAgent = (*agentsList)[i];

        if (CheckIsInBounds(bounds, currentAgent->position))
        {
            agentCatchs.AddBack(currentAgent);
            currentAgent->TakeDamage();
        }
    }

    int finalScore = CheckAddScore(agentCatchs);

    if (finalScore > 0)
        popUpScoreList.push_back(new PopUpScore(currentPlayer->position, finalScore, &gameManager, currentPlayer->ID));
    gameManager.AddScore(playerWhoDo, CheckAddScore(agentCatchs));
}

int CollisionManager::CheckAddScore(const DinoArray<Agent*>& list)
{
    DinoArray<int> numberAnimalTypeCatch;
    int finalScore = 0;

    for (int i = 0; i < 8; i++) numberAnimalTypeCatch.AddBack(0);
    
    for (size_t i = 0; i < list.GetSize(); i++)
    {
        numberAnimalTypeCatch[list[i]->typeAgent] += 1;
    }

    for (size_t i = 0; i < numberAnimalTypeCatch.GetSize(); i++)
    {
        for (size_t j = 1; j <= numberAnimalTypeCatch[i]; j++)
        {
            finalScore += 10 * j;
        }
    }

    return finalScore;
}

bool CollisionManager::CheckIsInBounds(const DinoArray<DinoVec2>& bounds, DinoVec2 pos)
{
    int intersections = 0;
    size_t count = bounds.GetSize();
    DinoVec2 rayEnd = {1e9f, pos.y};

    for (size_t i = 0; i < count; i++)
    {
        DinoVec2 a = bounds[i];
        DinoVec2 b = bounds[(i + 1) % count];

        if (Dino_IntersectSegment(pos, rayEnd, a, b))
            intersections++;
    }

    return (intersections % 2) == 1;
}

void CollisionManager::SetAgents(DinoArray<Agent*>* agents) { agentsList = agents; }
void CollisionManager::SetAnimals(DinoArray<Animal*>* animals) { animalList = animals; }
void CollisionManager::SetPlayers(DinoArray<DinoPlayer*>* players) { playerList = players; }
