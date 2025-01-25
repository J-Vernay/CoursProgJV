#include <dino/dino_lasso.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_geometry.h>

void DinoLasso::Init(DinoColor color)
{
    m_color = color;
}

void DinoLasso::Update(DinoVec2 newPoint)
{
    m_points.push_back(newPoint);
    if (m_points.size() > 120)
        m_points.erase(m_points.begin());
}

void DinoLasso::Draw() const
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(m_points, 4, m_color);
    XDino_Draw(drawCall);
}

void DinoLasso::ApplyCollisionSelf()
{
    size_t lassoSize = m_points.size();
    if (lassoSize < 4)
        return;
    // On n'a besoin de regarder que le dernier segment qui a été rajouté.
    DinoVec2 C = m_points[lassoSize - 2];
    DinoVec2 D = m_points[lassoSize - 1];
    for (size_t i = 0; i < lassoSize - 3; ++i) {
        DinoVec2 A = m_points[i];
        DinoVec2 B = m_points[i + 1];
        if (Dino_IntersectSegment(A, B, C, D)) {
            m_points.erase(m_points.begin() + (i + 1), m_points.begin() + (lassoSize - 1));
            break;
        }
    }
}

void DinoLasso::ApplyCollisionTo(DinoLasso& other) const
{
    // Est-ce que le dernier segment rajouté collisionne avec l'un des autres lassos?
    size_t myLassoSize = m_points.size();
    if (myLassoSize < 2)
        return;
    DinoVec2 C = m_points[myLassoSize - 2];
    DinoVec2 D = m_points[myLassoSize - 1];

    size_t otherLassoSize = other.m_points.size();
    if (otherLassoSize < 2)
        return;
    for (size_t i = 0; i < otherLassoSize - 2; ++i) {
        DinoVec2 A = other.m_points[i];
        DinoVec2 B = other.m_points[i + 1];
        if (Dino_IntersectSegment(A, B, C, D)) {
            other.m_points.erase(other.m_points.begin(), other.m_points.begin() + i);
            break;
        }
    }

}