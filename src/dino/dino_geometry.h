/// @file dino_geometry.h
/// @brief Fonctions de calculs de collisions.

#pragma once

#include <dino/xdino.h>

/// Vérifie si les deux segments [AB] et [CD] ont une intersection.
bool Dino_IntersectSegment(DinoVec2 a, DinoVec2 b, DinoVec2 c, DinoVec2 d);

/// Détecte et résout une intersection entre deux cercles.
bool Dino_CollideCircles(DinoVec2* a, DinoVec2* b, float collisionDistance);