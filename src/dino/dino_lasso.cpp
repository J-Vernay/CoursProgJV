#include "dino_draw_utils.h"
#include "dino_geometry.h"
#include <dino/dino_draw_utils.h>

#include <dino/dino_lasso.h>


DinoLasso::DinoLasso(DinoColor color, int ownerIndex)
{
    m_color = color;
    m_ownerIndex = ownerIndex;
}


void DinoLasso::Update(DinoVec2 pos)
{
    constexpr float UPDATE_PER_SECONDE = 60.f;
    constexpr float LASSO_TIME = 2.f;

    m_lasso.push_back(pos);
    if (m_lasso.size() > LASSO_TIME * UPDATE_PER_SECONDE)
        m_lasso.erase(m_lasso.begin());

    m_loopLasso.clear();
    if (m_lasso.size() < 4)
        return;
    DinoVec2 c = m_lasso[m_lasso.size() - 2];
    DinoVec2 d = m_lasso[m_lasso.size() - 1];
    if (c.x != d.x || c.y != d.y) {
        for (int i = 0; i < m_lasso.size() - 3; i++) {
            DinoVec2 a = m_lasso[i];
            DinoVec2 b = m_lasso[i + 1];
            if (Dino_IntersectSegment(a, b, c, d)) {
                m_loopLasso.assign(m_lasso.begin() + i + 1, m_lasso.end() - 1);
                m_lasso.erase(m_lasso.begin() + i + 1, m_lasso.end() - 1);
                break;
            }
        }
    }

}

bool DinoLasso::WasInLoop(DinoVec2 pos)
{
    if (m_loopLasso.size() < 3)
        return false;

    DinoVec2 origin = {0, 0};

    int nbCollisions = 0;
    for (int i = 0; i < m_loopLasso.size(); ++i) {
        DinoVec2 c = m_loopLasso[i];
        DinoVec2 d = (i == m_loopLasso.size() - 1) ? m_loopLasso[0] : m_loopLasso[i + 1];
        if (Dino_IntersectSegment(origin, pos, c, d))
            nbCollisions += 1;
    }

    return (nbCollisions % 2) == 1;
}


void DinoLasso::Draw()
{
    std::vector<DinoVertex> vs;
    Dino_GenVertices_Polyline(vs, m_lasso, 5, m_color);
    DinoVertexBuffer vbuf = {vs.data(), vs.size(), "lasso"};
    XDino_Draw(vbuf.GetVbufID(), XDino_TEXID_WHITE);
}

void DinoLasso::ResolveCollision(DinoLasso& lassoA, DinoLasso& lassoB)
{
    if (lassoA.m_lasso.size() < 2 || lassoB.m_lasso.size() < 2)
        return;

    std::vector<DinoVec2>& pointsA = lassoA.m_lasso;
    std::vector<DinoVec2>& pointsB = lassoB.m_lasso;

    DinoVec2 a = pointsA[pointsA.size() - 2];
    DinoVec2 b = pointsA[pointsA.size() - 1];
    for (int i = 0; i < pointsB.size() - 1; ++i) {
        DinoVec2 c = pointsB[i];
        DinoVec2 d = pointsB[i + 1];
        if (Dino_IntersectSegment(a, b, c, d)) {
            pointsB.erase(pointsB.begin(), pointsB.begin() + i);
            break;
        }
    }

    a = pointsB[pointsB.size() - 2];
    b = pointsB[pointsB.size() - 1];
    for (int i = 0; i < pointsA.size() - 1; ++i) {
        DinoVec2 c = pointsA[i];
        DinoVec2 d = pointsA[i + 1];
        if (Dino_IntersectSegment(a, b, c, d)) {
            pointsA.erase(pointsA.begin(), pointsA.begin() + i);
            break;
        }
    }

}