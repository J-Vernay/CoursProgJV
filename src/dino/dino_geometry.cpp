#include <dino/dino_geometry.h>
#include <cmath>

bool Dino_IntersectSegment(DinoVec2 A, DinoVec2 B, DinoVec2 C, DinoVec2 D)
{
    if (A.x == B.x && A.y == B.y)
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
    else
        return 0 <= dot_AB_AC && dot_AB_AD <= dot_AB_AB;
}

void Dino_ResolveCircleCollision(DinoVec2& a, DinoVec2& b, float radius)
{
    float ab_sq = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    float r_sq = radius * radius;
    if (ab_sq >= 4 * r_sq)
        return; // Les deux cercles ne sont pas en collision.
    if (ab_sq == 0)
        return; // Les deux cercles sont identiques, pas de résolution possible.
    float ab = sqrt(ab_sq);
    float alpha = (2 * radius - ab) / (2 * ab);
    DinoVec2 v;
    v.x = alpha * (b.x - a.x);
    v.y = alpha * (b.y - a.y);
    a.x -= v.x;
    a.y -= v.y;
    b.x += v.x;
    b.y += v.y;
}