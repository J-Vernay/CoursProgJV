#pragma once

#include <dino/xdino.h>
#include <dino/Entities/dino_Entity.h>
#include <algorithm>

class dino_EntityManager {
public :
    std::vector<dino_Entity*> entities;

    void UpdateAndDrawEntities(double timeSinceStart, float deltaTime);
    void DrawEntitiesWithTerrainClamping(double timeSinceStart, DinoVec2 terrainTopLeft);
    void DinoCollision_HandleCollisions(DinoVec2 terrainTopLeft);
    void AddEntity(dino_Entity* entity);
    void RemoveEntity(dino_Entity& entity);

private :
    void ResolveEntityCollision(dino_Entity* e1, dino_Entity* e2);
};