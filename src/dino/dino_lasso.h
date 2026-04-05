#pragma once

#include <dino/xdino.h>

class DinoLasso {
private:
    std::vector<DinoVec2> m_lasso;
    DinoColor m_color;
    std::vector<DinoVec2> m_loopLasso;

public:
    DinoLasso(DinoColor color);
    void Update(DinoVec2 pos);
    bool WasInLoop(DinoVec2 pos);
    bool HasLoop() const;
    void Draw();

    static void ResolveCollision(DinoLasso& lassoA, DinoLasso& lassoB);
};