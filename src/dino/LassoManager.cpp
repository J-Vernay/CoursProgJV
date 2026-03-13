#include "LassoManager.h"

#include "dino_geometry.h"

void LassoManager::SimpleDrawLasso()
{
    for (dino_lasso& lasso : lassos)
        lasso.SimpleDrawLasso();
}

void LassoManager::UpdateLassos(std::vector<dino_Entity*>& entities)
{
    for (int i = lassos.size() - 1; i >= 0; i--) {
        lassos[i].UpdateLasso(entities);
    }

    CheckLassoCollision();
}

void LassoManager::AddLasso(dino_player& player)
{
    lassos.emplace_back(player);
}

void LassoManager::RemovePlayer(dino_player& player)
{
    for (int i = lassos.size() - 1; i >= 0; i--) {
        if (lassos[i].IsMyPlayer(player)) {
            lassos.erase(lassos.begin() + i);
        }
    }
}

void LassoManager::CheckLassoCollision()
{
    for (int i = 0; i < lassos.size(); i++) {
        if (lassos[i].lassoPoints.size() < 4)
            continue;

        DinoVec2 A = lassos[i].lassoPoints[lassos[i].lassoPoints.size() - 2];
        DinoVec2 B = lassos[i].lassoPoints[lassos[i].lassoPoints.size() - 1];

        for (int j = 0; j < lassos.size(); j++) {
            if (lassos[j].lassoPoints.size() < 4)
                continue;

            for (int k = 0; k < lassos[j].lassoPoints.size() - 4; k++) {
                DinoVec2 C = lassos[j].lassoPoints[k];
                DinoVec2 D = lassos[j].lassoPoints[k + 1];
                if (lassos[j].ArePointsEqual(C, D))
                    continue;

                if (Dino_IntersectSegment(A, B, C, D)) {
                    lassos[j].CutLasso(k);
                    break;
                }
            }
        }
    }
}