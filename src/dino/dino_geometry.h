#pragma once

#include <dino/xdino.h>

/// Vérifie si les deux segments [AB] et [CD] ont une intersection.
bool Dino_IntersectSegment(DinoVec2 a, DinoVec2 b, DinoVec2 c, DinoVec2 d);

bool Dino_IntersectSegment(DinoVec2 a, DinoVec2 b, DinoVec2 c, DinoVec2 d);
/// Résout la collision entre deux points.
/// 'a' et 'b' seront modifiés s'ils sont en collision.
void Dino_ResolveCircleCollision(DinoVec2& a, DinoVec2& b, float radius);