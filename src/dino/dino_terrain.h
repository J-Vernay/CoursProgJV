#pragma once

#include <dino/xdino.h>

class DinoTerrain {
public:
    void Init(DinoVec2 rdrSize, int idxSeason);
    void Draw();
    void Shut();

private:
    uint64_t m_vbufID = 0;
    uint64_t m_texID = 0;
};