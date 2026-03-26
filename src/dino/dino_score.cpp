#include "dino_draw_utils.h"

#include <format>
#include <dino/dino_score.h>

constexpr int SCORE_DISPLAY_OFFSET = 15;

void DinoScore::Init(int player, DinoColor color)
{
    m_player = player;
    m_color = color;
}

void DinoScore::Reset()
{
    m_score = 0;
}

void DinoScore::AddScore(int score)
{
    m_score += score;
}

void DinoScore::DrawScore()
{
    if (!isInGame)
        return;

    std::string text = std::format("P{} : {:0>4}", m_player, m_score);
    std::vector<DinoVertex> vs;
    DinoColor bg = {DinoColor_WHITE.r, DinoColor_WHITE.g, DinoColor_WHITE.b, 100};
    Dino_GenVertices_Text(vs, text, m_color, bg, {0, (float)((3 + m_player) * SCORE_DISPLAY_OFFSET)});
    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "PlayerScore");
    XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 1.5);
    XDino_DestroyVertexBuffer(vbufID);
}