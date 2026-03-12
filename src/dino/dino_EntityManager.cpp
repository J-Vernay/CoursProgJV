#include "dino_EntityManager.h"
#include "dino_animal.h"

void dino_EntityManager::UpdateAndDrawEntities(double timeSinceStart, float deltaTime)
{
    std::sort(entities.begin(), entities.end(), dino_Entity::CompareVerticalY);

    for (int i = entities.size() - 1; i >= 0; i--) {
        if (entities[i]->IsEntityDead()) {
            entities.erase(entities.begin() + i);
            continue;
        }
        entities[i]->Update(deltaTime);
        entities[i]->DrawEntity(timeSinceStart);
    }
}

void dino_EntityManager::DinoCollision_HandleCollisions(DinoVec2 terrainTopLeft)
{
    if (entities.empty())
        return;

    for (int i = 0; i < entities.size() - 1; i++) {
        for (int j = i + 1; j < entities.size(); j++) {
            ResolveEntityCollision(entities[i], entities[j]);
            entities[i]->MoveIfOutOfBounds(terrainTopLeft);
            entities[j]->MoveIfOutOfBounds(terrainTopLeft);
        }
    }
}

void dino_EntityManager::AddEntity(dino_Entity* entity)
{
    entities.push_back(entity);
}

void dino_EntityManager::ResolveEntityCollision(dino_Entity* e1, dino_Entity* e2)
{
    float abPow = (e2->entityPosition.x - e1->entityPosition.x) * (e2->entityPosition.x - e1->entityPosition.x) + (
                      e2->entityPosition.y - e1->entityPosition.y) * (e2->entityPosition.y - e1->entityPosition.y);

    float minDistance = e1->collisionRadius + e2->collisionRadius;

    if (abPow == 0 || abPow >= minDistance * minDistance)
        return;

    //preventing expensive square root calculation for most entities
    float ab = sqrt(abPow);

    float dx = (minDistance - ab) / (2 * ab) * (e2->entityPosition.x - e1->entityPosition.x);
    float dy = (minDistance - ab) / (2 * ab) * (e2->entityPosition.y - e1->entityPosition.y);

    e1->entityPosition.x -= dx;
    e1->entityPosition.y -= dy;
    e2->entityPosition.x += dx;
    e2->entityPosition.y += dy;
}