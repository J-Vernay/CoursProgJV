#pragma once

#include <dino/GameManager/DinoGameState.h>

#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>
#include <dino/Entities/dino_Entity.h>
#include "dino/GameManager/PlayingState.h"

#include <dino/xdino.h>

class dino_tree : public dino_Entity{
public :
    dino_tree(DinoVec2 pos, int season, uint64_t texId, DinoGameState* dino_game_state);
    void DrawEntity(double timeSinceStart) override;
    void LassoCatched(int playerId) override;

private:
    uint64_t treeTextureId;
    int treeSeason;
    DinoVec2 position;
    DinoGameState* m_dinoGameState; 
};