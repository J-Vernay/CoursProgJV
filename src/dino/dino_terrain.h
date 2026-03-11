#pragma once
#include "xdino.h"

#include <cstdint>

class DinoTerrain {
public :
    int m_idxSeason;
    DinoVec2 m_topLeft;
    void Init(DinoVec2 rdrSize, int idx);
    void Draw();
    void Update(double timeSinceStart);
    void Shut();

private :
    void DrawFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos);
    void DrawFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft);
    uint64_t CreateEgdes(int idxSeason, int idxAnim, DinoVec2 topLeft);
    void CreateEdgeTiles(std::vector<DinoVertex>& out, int indexSeason, DinoVec2 topLeft, int idxAnim, int tx, int ty);
    uint64_t m_vbufID;
    uint64_t m_texID;
};