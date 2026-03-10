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

    void GenFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft);
    void GenFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos);
};