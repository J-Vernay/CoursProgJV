#include <dino/dino_geometry.h>

bool Dino_IntersectSegment(DinoVec2 A, DinoVec2 B, DinoVec2 C, DinoVec2 D)
{
    DinoVec2 AB = {B.x - A.x, B.y - A.y};
    DinoVec2 CD = {D.x - C.x, D.y - C.y};
    // Deux inconnues (alpha, beta), deux équations (axe X, axe Y). On veut savoir si alpha et beta sont entre 0 et 1.
    // A + alpha * AB = C + beta * CD

    float alpha_zABCD = CD.y * (C.x - A.x) - CD.x * (C.y - A.y);
    float beta_zABCD = AB.y * (C.x - A.x) - AB.x * (C.y - A.y);
    float zABCD = CD.y * AB.x - CD.x * AB.y;

    if (zABCD >= 0) {
        return 0 <= alpha_zABCD && alpha_zABCD <= zABCD && 0 <= beta_zABCD && beta_zABCD <= zABCD;
    }
    else {
        return 0 >= alpha_zABCD && alpha_zABCD >= zABCD && 0 >= beta_zABCD && beta_zABCD >= zABCD;
    }
}