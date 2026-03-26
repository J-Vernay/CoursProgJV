#include <dino/dino_tree.h>

uint64_t DinoTree::s_texID;

void DinoTree::ReactLimit(bool xChanged)
{
}

void DinoTree::ReactLoop(double timeSinceStart, int score, DinoColor lassoColor)
{
    if (m_unusableTimer > 0.f)
        return;

    m_bWasLooped = true;
}

DinoTree::DinoTree(DinoVec2 pos, int idxSeason, float timeToBeUnusable)
{
    m_pos = pos;
    m_idxSeason = idxSeason;
    m_bWasLooped = false;
    m_unusableTimer = timeToBeUnusable;
}

void DinoTree::Update(double timeSinceStart, float deltaTime)
{
    if (m_unusableTimer > 0.f) {
        m_unusableTimer -= deltaTime;
    }
}

void DinoTree::Draw(double timeSinceStart)
{
    std::vector<DinoVertex> vs;

    uint16_t umin = 48 + m_idxSeason * 80;
    uint16_t umax = 80 + m_idxSeason * 80;
    uint16_t vmin = 16;
    uint16_t vmax = 64;

    uint8_t alpha = m_unusableTimer > 0.f ? 150 : 255;
    DinoColor color{255, 255, 255, alpha};

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin;
    vs[0].v = vmin;
    vs[0].color = color;
    vs[1].pos = {32, 0};
    vs[1].u = umax;
    vs[1].v = vmin;
    vs[1].color = color;
    vs[2].pos = {0, 48};
    vs[2].u = umin;
    vs[2].v = vmax;
    vs[2].color = color;
    vs[3].pos = {32, 0};
    vs[3].u = umax;
    vs[3].v = vmin;
    vs[3].color = color;
    vs[4].pos = {0, 48};
    vs[4].u = umin;
    vs[4].v = vmax;
    vs[4].color = color;
    vs[5].pos = {32, 48};
    vs[5].u = umax;
    vs[5].v = vmax;
    vs[5].color = color;

    DinoVertexBuffer vbuf(vs.data(), vs.size(), "Tree");
    XDino_Draw(vbuf.Get(), s_texID, {m_pos.x - 16, m_pos.y - 40});
}

bool DinoTree::WasLooped()
{
    return m_bWasLooped;
}

int DinoTree::GetIdxSeason()
{
    return m_idxSeason;
}

void DinoTree::InitTexture()
{
    s_texID = XDino_CreateGpuTexture("terrain.png");
}

void DinoTree::ShutTexture()
{
    XDino_DestroyGpuTexture(s_texID);
}