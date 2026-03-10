#pragma once
#include "xdino.h"

#include <cstdint>

class DinoTerrain {
public :
    void Init(DinoVec2 rdrSize, int idx);
    void Draw();
    void Shut();

private :
    void DrawFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos);
    void DrawFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft);

    uint64_t m_vbufID;
    uint64_t m_texID;
};