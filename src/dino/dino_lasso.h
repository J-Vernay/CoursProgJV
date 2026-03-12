#pragma once

#include "dino_player.h"

#include <dino/xdino.h>

class dino_lasso {
public :
    std::vector<DinoVec2> lassoPoints;
    dino_lasso(dino_player& player);
    void UpdateLasso(std::vector<dino_Entity*>& entities);
    void CutLasso(int fromIndex);
    static bool ArePointsEqual(DinoVec2 p1, DinoVec2 p2);
    bool IsPointInLoop(DinoVec2 p, int index1, int index2);

private:
    dino_player* attachedPlayer;
};