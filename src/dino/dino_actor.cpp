/// @file dino_actor.cpp
/// @brief Implémentation de la classe DinoActor.


#include <dino/xdino.h>
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
    }

    if (position.x + ACTOR_SIZE > terrainMax.x) {
        position.x = terrainMax.x - ACTOR_SIZE;
    }

    if (position.y - ACTOR_SIZE < terrainMin.y) {
        position.y = terrainMin.y + ACTOR_SIZE;
    }

    if (position.y + ACTOR_SIZE > terrainMax.y) {
        position.y = terrainMax.y - ACTOR_SIZE;
    }
}