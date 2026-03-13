#pragma once

#include <dino/xdino.h>

class DinoLasso {
private:
    std::vector<DinoVec2> m_lasso;
    std::vector<DinoVec2> m_loopLasso;

    DinoColor m_color;

public:
    void Init(DinoColor color);
    void Update(DinoVec2 pos);
    bool WasInLoop(DinoVec2 pos);
    void Draw();

    static void ResolveCollision(DinoLasso& lassoA, DinoLasso& lassoB);
};