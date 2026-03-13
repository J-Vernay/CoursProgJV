#include <dino/dino_tree.h>

uint64_t DinoTree::s_texID;


void DinoTree::ReactLoop(double timeSinceStart)
{
    m_bWasLooped = true;
}

DinoTree::DinoTree(DinoVec2 pos, int idxSeason)
{
    m_pos = pos;
    m_idxSeason = idxSeason;
    m_bWasLooped = false;
}

void DinoTree::Update(double timeSinceStart, float deltaTime)
{
}

void DinoTree::Draw(double timeSinceStart)
{
    std::vector<DinoVertex> vs;

    uint16_t umin = 48 + m_idxSeason * 80;
    uint16_t umax = 80 + m_idxSeason * 80;
    uint16_t vmin = 16;
    uint16_t vmax = 64;

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin;
    vs[0].v = vmin;
    vs[1].pos = {32, 0};
    vs[1].u = umax;
    vs[1].v = vmin;
    vs[2].pos = {0, 48};
    vs[2].u = umin;
    vs[2].v = vmax;
    vs[3].pos = {32, 0};
    vs[3].u = umax;
    vs[3].v = vmin;
    vs[4].pos = {0, 48};
    vs[4].u = umin;
    vs[4].v = vmax;
    vs[5].pos = {32, 48};
    vs[5].u = umax;
    vs[5].v = vmax;

    DinoVertexBuffer vbuf(vs.data(), vs.size(), "Tree");
    XDino_Draw(vbuf.GetVbufID(), s_texID, {m_pos.x - 16, m_pos.y - 40});
}

bool DinoTree::WasLooped()
{
    return m_bWasLooped;
}

int DinoTree::GetIdxSeason()
{
    return m_idxSeason;
}

void DinoTree::InitStatic()
{
    s_texID = XDino_CreateGpuTexture("terrain.png");
}

void DinoTree::ShutStatic()
{
    XDino_DestroyGpuTexture(s_texID);
}