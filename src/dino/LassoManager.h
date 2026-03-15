#pragma once

#include "dino_lasso.h"
#include <dino/Entities/dino_player.h>
#include <dino/xdino.h>

class LassoManager {
public :
    void SimpleDrawLasso();
    void UpdateLassos(std::vector<dino_Entity*>& entities);
    void AddLasso(dino_player& player);
    void RemovePlayer(dino_player& player);

private:
    std::vector<dino_lasso> lassos;

    void CheckLassoCollision();
};