#include <dino/Entities/dino_tree.h>

#include <iostream>

dino_tree::dino_tree(DinoVec2 pos, int season, uint64_t texId, DinoGameState* dino_game_state)
{
    entityPosition = pos;
    collisionRadius = 1;
    
    treeSeason = season;
    position = pos;
    treeTextureId = texId;
    m_dinoGameState = dino_game_state;
}

void dino_tree::DrawEntity(double timeSinceStart)
{
    float Uu = 48 + 80*treeSeason;
    float Uv = 16;
    
    std::vector<DinoVertex> vs;
    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].color = DinoColor_WHITE;
    vs[1].pos = {2, 0};
    vs[1].color = DinoColor_WHITE;
    vs[2].pos = {0, 3};
    vs[2].color = DinoColor_WHITE;
    vs[3].pos = {2, 0};
    vs[3].color = DinoColor_WHITE;
    vs[4].pos = {0, 3};
    vs[4].color = DinoColor_WHITE;
    vs[5].pos = {2, 3};
    vs[5].color = DinoColor_WHITE;
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
    XDino_Draw(vb.Get(), treeTextureId, position-DinoVec2{10,15}, 10);
    
#if !XDINO_RELEASE
    //DEBUG ONLY
    vs.clear();
    Dino_GenVertices_Circle(vs, 10);
    DinoVertexBuffer vb2(vs.data(), vs.size(), "selectTreeDebug");
    XDino_Draw(vb2.Get(), XDino_TEXID_FONT, position, 1);
#endif
}

void dino_tree::LassoCatched(int playerId)
{
    m_dinoGameState->ChangeGameState(std::make_unique<PlayingState>(m_dinoGameState, treeSeason));
}