#pragma once

#include <dino/xdino.h>

class DinoTerrain {
public:
    void Init(DinoVec2 rdrSize, int idxSeason);
    void Draw(double timeSinceStart);
    void Shut();

    DinoVec2 GetTopLeft();
    DinoVec2 GetBottomRight();

private:
    uint64_t m_vbufID = 0;
    uint64_t m_texID = 0;
    int m_idxSeason;
    float m_dx = 0;
    float m_dy = 0;

    void GenFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft);
    void GenFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos);

    uint64_t CreateEgdes(int idxSeason, int idxAnim, DinoVec2 topLeft);
    void CreateEdgeTiles(std::vector<DinoVertex>& out, int indexSeason, DinoVec2 topLeft, int idxAnim, int tx, int ty);
};