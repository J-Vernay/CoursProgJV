#include <dino/Entities/dino_tree.h>
#include <dino/GameManager/DinoGameState.h>

//prevent circular dependency
#include <dino/GameManager/PlayingState.h>


#include <iostream>

dino_tree::dino_tree(DinoVec2 pos, int season, uint64_t texId, DinoGameState* dino_game_state, double spawn_time)
{
    entityPosition = pos;
    collisionRadius = 1;

    treeSeason = season;
    position = pos;
    treeTextureId = texId;
    m_dinoGameState = dino_game_state;
    spawnTime = spawn_time;
    canBeActivated = false;
}

void dino_tree::DrawEntity(double timeSinceStart)
{
    canBeActivated = timeSinceStart > spawnTime + 5;
    DinoColor SpriteColor = canBeActivated ? DinoColor_WHITE : DinoColor_TRANSPARENT;

    float Uu = 48 + 80 * treeSeason;
    float Uv = 16;

    std::vector<DinoVertex> vs;
    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].color = SpriteColor;
    vs[1].pos = {2, 0};
    vs[1].color = SpriteColor;
    vs[2].pos = {0, 3};
    vs[2].color = SpriteColor;
    vs[3].pos = {2, 0};
    vs[3].color = SpriteColor;
    vs[4].pos = {0, 3};
    vs[4].color = SpriteColor;
    vs[5].pos = {2, 3};
    vs[5].color = SpriteColor;
    vs[0].u = Uu;
    vs[0].v = Uv;
    vs[1].u = Uu + 32;
    vs[1].v = Uv;
    vs[2].u = Uu;
    vs[2].v = Uv + 48;
    vs[3].u = Uu + 32;
    vs[3].v = Uv;
    vs[4].u = Uu;
    vs[4].v = Uv + 48;
    vs[5].u = Uu + 32;
    vs[5].v = Uv + 48;

    DinoVertexBuffer vb(vs.data(), vs.size(), "selectTree");
    XDino_Draw(vb.Get(), treeTextureId, position - DinoVec2{10, 15}, 10);
}

void dino_tree::LassoCatched(int playerId)
{
    if (canBeActivated) {
        m_dinoGameState->ChangeGameState(std::make_unique<PlayingState>(m_dinoGameState, treeSeason));
    }
}