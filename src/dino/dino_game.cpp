#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_terrain.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>
#include <dino/dino_tree.h>

#include "dino_gamestate.h"
#include "dino_lobbystate.h"

#include <format>
#include <optional>
#include <vector>

constexpr DinoVec2 RENDER_SIZE = {480, 360};

double                       g_lastTime = 0;
std::vector<DinoGamepadIdx>  g_unassignedGamepads;
std::vector<PlayerState>     g_players;
DinoTerrain                  g_terrain;
GameState*                   g_currentState = nullptr;

std::optional<DinoVertexBuffer> g_vbufID_prenom;
DinoVec2                        g_textSize_prenom;

int g_debugScroll = 0;


void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);

    DinoPlayer::InitStatic();
    DinoAnimal::InitStatic();
    DinoTree::InitStatic();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        g_unassignedGamepads.emplace_back(idx);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_terrain.Init(RENDER_SIZE, idxSeason);

    {
        std::vector<DinoVertex> vs;
        g_textSize_prenom = Dino_GenVertices_Text(vs, "Antoine BOULANGER", DinoColor_WHITE, DinoColor_GREY);
        g_vbufID_prenom.emplace(vs.data(), vs.size(), "Prenom");
    }

    GameContext ctx{g_unassignedGamepads, g_players, g_terrain, RENDER_SIZE};
    g_currentState = new LobbyState(0.0, ctx);
}

void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    XDino_SetRenderSize(RENDER_SIZE);

    GameContext ctx{g_unassignedGamepads, g_players, g_terrain, RENDER_SIZE};

    GameState* nextState = g_currentState->Update(timeSinceStart, deltaTime, ctx);
    if (nextState) {
        delete g_currentState;
        g_currentState = nextState;
    }

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    g_currentState->Draw(timeSinceStart, ctx);

    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {}, 2);
    }

    {
        float tx = RENDER_SIZE.x - g_textSize_prenom.x * 2;
        float ty = RENDER_SIZE.y - g_textSize_prenom.y * 2;
        XDino_Draw(g_vbufID_prenom->Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

#if !XDINO_RELEASE
    DinoGamepad keyboard;
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard) && keyboard.shoulder_left)
        XDino_DrawStats(keyboard.dpad_up ? -1 : keyboard.dpad_down ? 1 : 0);
#endif
}

void Dino_GameShut()
{
    delete g_currentState;
    g_currentState = nullptr;

    for (PlayerState& player : g_players)
        player.dino.Shut();

    g_terrain.Shut();
    DinoTree::ShutStatic();
    DinoPlayer::ShutStatic();
    DinoAnimal::ShutStatic();

    g_vbufID_prenom.reset();
}