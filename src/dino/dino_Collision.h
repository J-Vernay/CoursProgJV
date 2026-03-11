#pragma once

#include <dino/xdino.h>
#include <dino/dino_CollisionEntity.h>

class dino_Collision {
public :
    void DinoCollision_HandleCollisions();
    void AddEntity(dino_CollisionEntity* entity);

private :
    std::vector<dino_CollisionEntity*> collisionEntities;

    void ResolveEntityCollision(dino_CollisionEntity* e1, dino_CollisionEntity* e2);
};