#pragma once


#include <dino/xdino.h>
#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>
#include <dino/Entities/dino_Entity.h>

//forward declaration
class DinoGameState;

class dino_tree : public dino_Entity {
public :
    dino_tree(DinoVec2 pos, int season, uint64_t texId, DinoGameState* dino_game_state, double spawn_time);
    void DrawEntity(double timeSinceStart) override;
    void LassoCatched(int playerId) override;

private:
    uint64_t treeTextureId;
    double spawnTime;
    bool canBeActivated;
    int treeSeason;
    DinoVec2 position;
    DinoGameState* m_dinoGameState;
};