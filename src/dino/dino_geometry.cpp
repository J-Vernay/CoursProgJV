#include <dino/dino_geometry.h>

bool Dino_IntersectSegment(DinoVec2 A, DinoVec2 B, DinoVec2 C, DinoVec2 D)
{
    bool bIntersect = true;

    // Axe X
    if (A.x <= B.x) {
        if (D.x <= C.x)
            bIntersect &= A.x <= C.x && D.x <= B.x;
        else
            bIntersect &= A.x <= D.x && C.x <= B.x;
    }
    else {
        if (D.x <= C.x)
            bIntersect &= B.x <= C.x && D.x <= A.x;
        else
            bIntersect &= B.x <= D.x && C.x <= A.x;
    }

    // Axe Y
    if (A.y <= B.y) {
        if (D.y <= C.y)
            bIntersect &= A.y <= C.y && D.y <= B.y;
        else
            bIntersect &= A.y <= D.y && C.y <= B.y;
    }
    else {
        if (D.y <= C.y)
            bIntersect &= B.y <= C.y && D.y <= A.y;
        else
            bIntersect &= B.y <= D.y && C.y <= A.y;
    }

    return bIntersect;
}