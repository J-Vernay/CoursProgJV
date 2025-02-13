/// @file dino_actor.cpp
/// @brief Implémentation de la classe DinoActor.

#include <dino/xdino.h>
#include <dino/dino_actor.h>

bool DinoActor::compareHeight(const DinoActor* first, const DinoActor* second)
{
    return first->position.y < second->position.y;
}