#include <dino/dino_lasso.h>
#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>

void DinoLasso::Init(DinoColor color)
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
                    m_lasso.erase(m_lasso.begin() + i + 1, m_lasso.end() - 1);
                    break;
                }
            }
        }
    }
}

void DinoLasso::Draw()
{
    /*
    DinoColor color;
    if (m_idxPlayer == 0)
        color = DinoColor_BLUE;
    else if (m_idxPlayer == 1)
        color = DinoColor_RED;
    else if (m_idxPlayer == 2)
        color = DinoColor_YELLOW;
    else
        color = DinoColor_GREEN;*/

    std::vector<DinoVertex> vs;
    Dino_GenVertices_Polyline(vs, m_lasso, 4, m_color);
    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Lasso");
    XDino_Draw(vbufID, XDino_TEXID_WHITE);
    XDino_DestroyVertexBuffer(vbufID);
}