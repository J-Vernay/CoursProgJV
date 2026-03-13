#include "dino_tree.h"

#include "dino_draw_utils.h"

dino_tree::dino_tree(DinoVec2 pos, int season, uint64_t texId)
{
    treeSeason = season;
    position = pos;
    treeTextureId = texId;
}

void dino_tree::DrawEntity(double timeSinceStart)
{
    float Uu = 48 + 96*treeSeason;
    float Uv = 16;
    
    std::vector<DinoVertex> vs;
    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].color = DinoColor_WHITE;
    vs[1].pos = {1, 0};
    vs[1].color = DinoColor_WHITE;
    vs[2].pos = {0, 1};
    vs[2].color = DinoColor_WHITE;
    vs[3].pos = {1, 0};
    vs[3].color = DinoColor_WHITE;
    vs[4].pos = {0, 1};
    vs[4].color = DinoColor_WHITE;
    vs[5].pos = {1, 1};
    vs[5].color = DinoColor_WHITE;
    vs[0].u = Uu;
    vs[0].v = Uv;
    vs[1].u = Uu + 48;
    vs[1].v = Uv;
    vs[2].u = Uu;
    vs[2].v = Uv + 72;
    vs[3].u = Uu + 48;
    vs[3].v = Uv;
    vs[4].u = Uu;
    vs[4].v = Uv + 72;
    vs[5].u = Uu + 48;
    vs[5].v = Uv + 72;
    
    DinoVertexBuffer vb(vs.data(), vs.size(), "selectTree");
    XDino_Draw(vb.Get(), treeTextureId, {}, 5);
}

void dino_tree::LassoCatched(int playerId)
{
    
}