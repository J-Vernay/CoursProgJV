#include "dino_entity.h"

#include <dino/dino_lasso.h>
#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>

DinoLasso::DinoLasso(DinoColor color)
{
    m_color = color;
}

void DinoLasso::Update(DinoVec2 pos)
{
    // 1. Je rajoute un point
    m_lasso.push_back(pos);

    // 2. Je m'assure d'avoir une taille max de lasso
    constexpr float UPDATE_PER_SECOND = 60;
    constexpr float TIME_LASSO = 2;
    if (m_lasso.size() > TIME_LASSO * UPDATE_PER_SECOND)
        m_lasso.erase(m_lasso.begin());

    m_loopLasso.clear();

    // 3. Détection d'une boucle ; juste besoin de tester le dernier segment ajouté.
    if (m_lasso.size() >= 4) {
        DinoVec2 c = m_lasso[m_lasso.size() - 2];
        DinoVec2 d = m_lasso[m_lasso.size() - 1];
        if (c.x != d.x || c.y != d.y) {
            for (int i = 0; i < m_lasso.size() - 3; ++i) {
                DinoVec2 a = m_lasso[i];
                DinoVec2 b = m_lasso[i + 1];
                if (Dino_IntersectSegment(a, b, c, d)) {
                    // Effacer le lasso du point B au point C (en gardant A et D relié)
                    // Il faut supprimer de i+1 (inclus) à size-2 (inclus)
                    // erase(first, last): first est inclus, last est exclus
                    m_loopLasso.assign(m_lasso.begin() + i + 1, m_lasso.end() - 1);
                    m_lasso.erase(m_lasso.begin() + i + 1, m_lasso.end() - 1);
                    break;
                }
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
    Dino_GenVertices_Polyline(vs, m_lasso, 4, m_color);
    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Lasso");
    XDino_Draw(vbufID, XDino_TEXID_WHITE);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoLasso::ResolveCollision(DinoLasso& lassoA, DinoLasso& lassoB)
{
    if (lassoA.m_lasso.size() < 2 || lassoB.m_lasso.size() < 2)
        return;

    std::vector<DinoVec2>& pointsA = lassoA.m_lasso;
    std::vector<DinoVec2>& pointsB = lassoB.m_lasso;

    // Dernier segment du lassoA avec tous les segments du lasso B
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

    // Dernier segment du lassoB avec tous les segments du lasso A
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