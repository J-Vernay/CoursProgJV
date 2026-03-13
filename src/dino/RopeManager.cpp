#include "RopeManager.h"
#include "dino_player.h"
#include "CodeBase/dino_geometry.h"
std::vector<std::vector<DinoVec2>> RopeManager::allRopes = {};


void RopeManager::Update()
{

    
}

void RopeManager::ResolveCollision(DinoObject::DinoLasso& lassoA, DinoObject::DinoLasso& lassoB)
{
    if (lassoA.ropePoints.size() < 2 || lassoB.ropePoints.size() < 2)
        return;

    std::vector<DinoObject::PointInfo>& pointsA = lassoA.ropePoints;
    std::vector<DinoObject::PointInfo>& pointsB = lassoB.ropePoints;

    // Dernier segment du lassoA avec tous les segments du lasso B
    DinoVec2 a = pointsA[pointsA.size() - 2].pointPos;
    DinoVec2 b = pointsA[pointsA.size() - 1].pointPos;
    for (int i = 0; i < pointsB.size() - 1; ++i) {
        DinoVec2 c = pointsB[i].pointPos;
        DinoVec2 d = pointsB[i + 1].pointPos;
        if (Dino_IntersectSegment(a, b, c, d)) {
            pointsB.erase(pointsB.begin(), pointsB.begin() + i);
            break;
        }
    }

    // Dernier segment du lassoB avec tous les segments du lasso A
    a = pointsB[pointsB.size() - 2].pointPos;
    b = pointsB[pointsB.size() - 1].pointPos;
    for (int i = 0; i < pointsA.size() - 1; ++i) {
        DinoVec2 c = pointsA[i].pointPos;
        DinoVec2 d = pointsA[i + 1].pointPos;
        if (Dino_IntersectSegment(a, b, c, d)) {
            pointsA.erase(pointsA.begin(), pointsA.begin() + i);
            break;
        }
    }

}
