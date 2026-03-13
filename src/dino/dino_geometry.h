#pragma once

#include <dino/xdino.h>

/// Vérifie si les deux segments [AB] et [CD] ont une intersection.
bool Dino_IntersectSegment(DinoVec2 a, DinoVec2 b, DinoVec2 c, DinoVec2 d);

DinoVec2 operator+(DinoVec2 a, DinoVec2 b);
DinoVec2 operator-(DinoVec2 a, DinoVec2 b);
DinoVec2 operator*(DinoVec2 a, float b);
DinoVec2 operator*(float a, DinoVec2 b);