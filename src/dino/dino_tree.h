#pragma once


#include "dino_Entity.h"

#include <dino/xdino.h>

class dino_tree : public dino_Entity{
public :
    dino_tree(DinoVec2 pos, int season, uint64_t texId);
    void DrawEntity(double timeSinceStart) override;
    void LassoCatched(int playerId) override;

private:
    uint64_t treeTextureId;
    int treeSeason;
    DinoVec2 position;
};