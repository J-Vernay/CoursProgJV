#pragma once

#include <dino/xdino.h>


class DinoLasso {
public :
    DinoLasso(DinoColor color, int ownerIndex);
    void Update(DinoVec2 pos);
    void Draw();
    static void ResolveCollision(DinoLasso& lassoA, DinoLasso& lassoB);
    bool WasInLoop(DinoVec2 pos);

    int m_ownerIndex;

private:
    std::vector<DinoVec2> m_lasso;
    std::vector<DinoVec2> m_loopLasso;
    DinoColor m_color;

};