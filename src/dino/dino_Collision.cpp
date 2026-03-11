#include "dino_Collision.h"

void dino_Collision::DinoCollision_HandleCollisions()
{
    if (collisionEntities.empty())
        return;

    for (int i = 0; i < collisionEntities.size() - 1; i++) {
        for (int j = i + 1; j < collisionEntities.size(); j++) {
            ResolveEntityCollision(collisionEntities[i], collisionEntities[j]);
        }
    }
}

void dino_Collision::AddEntity(dino_CollisionEntity* entity)
{
    collisionEntities.push_back(entity);
}

void dino_Collision::ResolveEntityCollision(dino_CollisionEntity* e1, dino_CollisionEntity* e2)
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