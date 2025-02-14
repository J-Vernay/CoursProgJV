/// @file dino_geometry.cpp
/// @brief Implémentation des fonctions de collisions.

#include <dino/dino_geometry.h>

bool Dino_IntersectSegment(DinoVec2 A, DinoVec2 B, DinoVec2 C, DinoVec2 D)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    if (abs(A.x - B.x) < epsilon && abs(A.y - B.y) < epsilon || abs(C.x - D.x) < epsilon && abs(C.y - D.y) < epsilon)
        return false;

    float z_AB_AC = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    float z_AB_AD = (B.x - A.x) * (D.y - A.y) - (B.y - A.y) * (D.x - A.x);
    float z_CD_CA = (D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x - C.x);
    float z_CD_CB = (D.x - C.x) * (B.y - C.y) - (D.y - C.y) * (B.x - C.x);

    if (z_AB_AC * z_AB_AD > 0)
        return false; // CD separated on axis perpendicular to AB
    if (z_CD_CA * z_CD_CB > 0)
        return false; // AB separated on axis perpendicular to CD

    // What if ABCD are aligned?

    float dot_AB_AB = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
    float dot_AB_AC = (B.x - A.x) * (C.x - A.x) + (B.y - A.y) * (C.y - A.y);
    float dot_AB_AD = (B.x - A.x) * (D.x - A.x) + (B.y - A.y) * (D.y - A.y);

    if (dot_AB_AC <= dot_AB_AD)
        return 0 <= dot_AB_AD && dot_AB_AC <= dot_AB_AB;
    return 0 <= dot_AB_AC && dot_AB_AD <= dot_AB_AB;
}

bool Dino_CollideCircles(DinoVec2* A, DinoVec2* B, float collisionDistance)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    if (abs(A->x - B->x) < epsilon && abs(A->y - B->y) < epsilon)
        return false;
    
    DinoVec2 diff = {A->x - B->x, A->y - B->y};
    float distance = sqrtf(powf(A->x - B->x, 2) + powf(A->y - B->y, 2));
    if (distance > collisionDistance)
        return false;
    DinoVec2 normal = {diff.x / distance, diff.y / distance};
    float offset = (collisionDistance - distance) / 2;

    A->x += normal.x * offset;
    A->y += normal.y * offset;
    B->x -= normal.x * offset;
    B->y -= normal.y * offset;
    return true;
}