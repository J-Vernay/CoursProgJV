#pragma once

#include <dino/Entities/dino_player.h>

#include <dino/xdino.h>

class dino_lasso {
public :
    std::vector<DinoVec2> lassoPoints;
    dino_lasso(dino_player& player);
    void SimpleDrawLasso();
    void UpdateLasso(std::vector<dino_Entity*>& entities, double timeSinceStart);
    void CutLasso(int fromIndex);
    static bool ArePointsToClose(DinoVec2 p1, DinoVec2 p2);
    bool IsPointInLoop(DinoVec2 p, int index1, int index2);
    bool IsMyPlayer(dino_player& player);

private:
    std::vector<float> lassoPointsTime;
    dino_player* attachedPlayer;

    static float LenghtSQR(DinoVec2 p1, DinoVec2 p2);
    static constexpr float LASSO_POINT_LIVING_TIME = 2.0f;
};