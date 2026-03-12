#pragma once

#include <dino/xdino.h>


class DinoLasso {
public :
    std::vector<DinoVec2> m_lasso;
    DinoColor m_color;
    void Update(DinoVec2 pos);
    void Init(DinoColor color);
    void Draw();
    static void ResolveCollision(DinoLasso& lassoA, DinoLasso& lassoB);
    std::vector<DinoVec2> m_loopLasso;
    bool WasInLoop(DinoVec2 pos);
};