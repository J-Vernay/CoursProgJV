/// @file dino_actor.cpp
/// @brief Implémentation de `DinoActor`.

#include <dino/xdino.h>
#include <dino/dino_geometry.h>
#include <dino/dino_terrain.h>
#include <dino/dino_actor.h>

constexpr float ACTOR_SIZE = 4;

bool DinoActor::compareHeight(const DinoActor* first, const DinoActor* second)
{
    return first->position.y < second->position.y;
}

void DinoActor::handleTerrainCollision()
{
    DinoVec2 terrainMin = DinoTerrain::get_terrain_min_position();
    DinoVec2 terrainMax = DinoTerrain::get_terrain_max_position();

    if (position.x - ACTOR_SIZE < terrainMin.x) {
        position.x = terrainMin.x + ACTOR_SIZE;
        onTerrainCollide();
    }

    if (position.x + ACTOR_SIZE > terrainMax.x) {
        position.x = terrainMax.x - ACTOR_SIZE;
        onTerrainCollide();
    }

    if (position.y - ACTOR_SIZE < terrainMin.y) {
        position.y = terrainMin.y + ACTOR_SIZE;
        onTerrainCollide();
    }

    if (position.y + ACTOR_SIZE > terrainMax.y) {
        position.y = terrainMax.y - ACTOR_SIZE;
        onTerrainCollide();
    }
}

void DinoActor::handleActorCollision(DinoActor* other)
{
    if (!isInteractable() || !other->isInteractable()) return;
    Dino_CollideCircles(&position, &other->position, 16);
}

void DinoActor::handleActorCircled(std::vector<DinoVec2>::iterator first, std::vector<DinoVec2>::iterator second)
{
    if (!isInteractable()) return;
    
    int intersectionCount = 0;

    while (first < second - 1) {
        if (Dino_IntersectSegment({0, 0}, position, first[0], first[1])) {
            intersectionCount++;
        }
        ++first;
    }
    
    if ((intersectionCount & 1) == 0) return;
    hit();
}