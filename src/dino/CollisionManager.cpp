#include "CollisionManager.h"

#include <iostream>

#include "dino/DinoPlayer.h"
#include "dino/dino_geometry.h"

CollisionManager::CollisionManager() {}


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
    for (int i = (int)agentsList->GetSize() - 1; i >= 0; i--)
    {
        if ((*agentsList)[i]->ID == playerWhoDo)
            continue;
        
        Agent* currentAgent = (*agentsList)[i];

        if (CheckIsInBounds(bounds, currentAgent->position))
        {
            currentAgent->TakeDamage();
        }
    }
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
