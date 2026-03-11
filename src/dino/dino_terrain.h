#pragma once

#include <dino/xdino.h>

class DinoTerrain {
public:
    void Init(DinoVec2, int);
    void Draw();
    void Shut();

    static DinoVec2 Fence;
    static DinoVec2 FenceStart;

private:
    uint64_t m_vbufID = 0;
    uint64_t m_texID = 0;

    void GenFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft);
    void GenFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos);
};