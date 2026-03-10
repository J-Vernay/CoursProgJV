#pragma once

#include <dino/xdino.h>

class DinoTerrain {
public:
    void Init(DinoVec2 rdrSiz, int idxSeason);
    void Draw();
    void Update(double timeSinceStart);
    void Shut();
    uint64_t CreateEdges(int idxSeason, int idxAnim, DinoVec2 posTopLeft);
    void CreateEdgeTile(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft, int idxAnim, int tx, int ty);
    void SpawnFlowers(std::vector<DinoVertex>& out, int idxSeason, DinoVec2 posTopLeft);
    void SpawnFlower(std::vector<DinoVertex>& out, int idxSeason, int idxFlower, DinoVec2 pos);

private:
    int m_idxSeason;
    uint64_t m_vbufID;
    uint64_t m_texID;
    std::vector<uint64_t> m_vbufFlowerID;
    std::vector<uint64_t> m_texFlowerID;
    DinoVec2 m_posTopLeft;
};