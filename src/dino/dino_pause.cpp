#include <dino/dino_pause.h>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_game.h>

constexpr DinoVec2 RENDER_SIZE = {480, 360};

void Pause_Update(bool bPressedStart, bool& bWasStartPressed, EGameState& state)
{
    if (bPressedStart && !bWasStartPressed)
        state = EGameState::InGame;
    bWasStartPressed = bPressedStart;
}

void Pause_Draw(double timeSinceStart)
{
    std::vector<DinoVertex> vs;
    DinoVec2 textSize = Dino_GenVertices_Text(vs, "-- PAUSE --", DinoColor_WHITE, DinoColor_BLACK);
    DinoVertexBuffer vbuf(vs.data(), vs.size(), "Pause");
    float tx = (RENDER_SIZE.x - textSize.x * 6) / 2;
    float ty = (RENDER_SIZE.y - textSize.y * 6) / 2;
    XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 6);
}