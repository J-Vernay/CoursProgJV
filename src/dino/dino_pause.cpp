#include <dino/dino_pause.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_game.h>
#include <dino/xdino.h>
#include <format>

constexpr DinoVec2 RENDER_SIZE = {480, 360};

static int g_selectedOption = 0;
static bool g_bWasDpadUp = false;
static bool g_bWasDpadDown = false;
static bool g_bWasDpadLeft = false;
static bool g_bWasDpadRight = false;
static bool g_bWasConfirm = false;

constexpr const char* OPTION_LABELS[4] = {
    "Reprendre",
    "Recommencer",
    "Retour au lobby",
    "Chrono"
};

void Pause_Update(std::vector<PlayerState>& players,
                  double& chrono,
                  EGameState& state,
                  EPauseAction& action)
{
    action = EPauseAction::None;

    // Collecter les inputs de tous les joueurs
    bool bDpadUp = false, bDpadDown = false;
    bool bDpadLeft = false, bDpadRight = false;
    bool bConfirm = false;

    for (PlayerState& player : players) {
        DinoGamepad gp;
        if (XDino_GetGamepad(player.gamepadIdx, gp)) {
            bDpadUp = bDpadUp || gp.dpad_up;
            bDpadDown = bDpadDown || gp.dpad_down;
            bDpadLeft = bDpadLeft || gp.dpad_left;
            bDpadRight = bDpadRight || gp.dpad_right;
            bConfirm = bConfirm || gp.btn_down;
        }
    }

    // Navigation
    if (bDpadUp && !g_bWasDpadUp)
        g_selectedOption = (g_selectedOption - 1 + 4) % 4;
    if (bDpadDown && !g_bWasDpadDown)
        g_selectedOption = (g_selectedOption + 1) % 4;

    // Modifier le chrono
    if (g_selectedOption == 3) {
        if (bDpadLeft && !g_bWasDpadLeft)
            chrono -= 10.0;
        if (bDpadRight && !g_bWasDpadRight)
            chrono += 10.0;
    }

    // Confirmer
    if (bConfirm && !g_bWasConfirm) {
        switch (g_selectedOption) {
        case 0: // Reprendre
            action = EPauseAction::Resume;
            state = EGameState::InGame;
            break;
        case 1: // Recommencer
            action = EPauseAction::Restart;
            state = EGameState::InGame;
            break;
        case 2: // Retour au lobby
            action = EPauseAction::BackToLobby;
            state = EGameState::Lobby;
            break;
        case 3: // Chrono
            break;
        }
    }

    g_bWasDpadUp = bDpadUp;
    g_bWasDpadDown = bDpadDown;
    g_bWasDpadLeft = bDpadLeft;
    g_bWasDpadRight = bDpadRight;
    g_bWasConfirm = bConfirm;
}

void Pause_Draw(double timeSinceStart, double chrono)
{
    // Fond semi-transparent
    std::vector<DinoVertex> vs;
    DinoVec2 textSize = Dino_GenVertices_Text(vs, "-- PAUSE --", DinoColor_WHITE, DinoColor_BLACK);
    DinoVertexBuffer vbuf(vs.data(), vs.size(), "Pause");
    float tx = (RENDER_SIZE.x - textSize.x * 4) / 2;
    float ty = RENDER_SIZE.y / 4;
    XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 4);

    // Options du menu
    for (int i = 0; i < 4; i++) {
        std::string label;
        if (i == 3)
            label = std::format("Chrono: {:.0f}s  (< >)", chrono);
        else
            label = OPTION_LABELS[i];

        DinoColor color = (i == g_selectedOption) ? DinoColor_YELLOW : DinoColor_WHITE;
        DinoColor bg = (i == g_selectedOption) ? DinoColor_BLACK : DinoColor_TRANSPARENT;

        std::vector<DinoVertex> vsOpt;
        DinoVec2 optSize = Dino_GenVertices_Text(vsOpt, label, color, bg);
        DinoVertexBuffer vbufOpt(vsOpt.data(), vsOpt.size(), "PauseOpt");
        float optX = (RENDER_SIZE.x - optSize.x * 2) / 2;
        float optY = RENDER_SIZE.y / 2 + static_cast<float>(i * 24);
        XDino_Draw(vbufOpt.Get(), XDino_TEXID_FONT, {optX, optY}, 2);
    }
}