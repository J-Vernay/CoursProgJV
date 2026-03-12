#pragma once

#include <dino/xdino.h>

class DinoLasso {
private:
    std::vector<DinoVec2> m_lasso;
    DinoColor m_color;

public:
    void Init(DinoColor color);
    void Update(DinoVec2 pos);
    void Draw();
};