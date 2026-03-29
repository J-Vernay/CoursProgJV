#include "dino_pause.h"
#include <format>

int DinoPause::s_selection = 0;
double DinoPause::s_lastNavTime = 0;

void DinoPause::Update(double timeSinceStart, bool& bPause, bool& bLobby, double& chrono)
{
    DinoGamepad kb, gp;
    bool hasKb = XDino_GetGamepad(DinoGamepadIdx::Keyboard, kb);
    bool hasGp = XDino_GetGamepad(static_cast<DinoGamepadIdx>(0), gp);

    // --- NAVIGATION HAUT/BAS ---
    if (timeSinceStart > s_lastNavTime + 0.18) {
        bool up = (hasKb && kb.dpad_up) || (hasGp && (gp.dpad_up || gp.stick_left_y < -0.5f));
        bool down = (hasKb && kb.dpad_down) || (hasGp && (gp.dpad_down || gp.stick_left_y > 0.5f));

        if (up) {
            s_selection--;
            s_lastNavTime = timeSinceStart;
        }
        else
            if (down) {
                s_selection++;
                s_lastNavTime = timeSinceStart;
            }

        if (s_selection < 0)
            s_selection = 3;
        if (s_selection > 3)
            s_selection = 0;
    }

    // --- LOGIQUE SPECIFIQUE AU CHRONO (Index 1) ---
    if (s_selection == 1) {
        if (timeSinceStart > s_lastNavTime + 0.15) {
            // Délai un peu plus court pour le confort
            bool left = (hasKb && kb.dpad_left) || (hasGp && (gp.dpad_left || gp.stick_left_x < -0.5f));
            bool right = (hasKb && kb.dpad_right) || (hasGp && (gp.dpad_right || gp.stick_left_x > 0.5f));

            if (left) {
                chrono = std::max(0.0, chrono - 10.0); // On retire 10s (minimum 0)
                s_lastNavTime = timeSinceStart;
            }
            else if (right) {
                chrono += 10.0; // On ajoute 10s
                s_lastNavTime = timeSinceStart;
            }
        }
    }

    // --- VALIDATION ---
    bool validate = (hasKb && kb.btn_down) || (hasGp && gp.btn_down);
    if (validate) {
        if (s_selection == 0)
            bPause = false; // Reprendre
        // On ne fait rien pour le bouton Chrono ici, car il réagit aux flèches
    }
}

void DinoPause::Draw()
{
    std::string options[] = {"Reprendre", "Chrono +/-", "Retour au Lobby", "Recommencer"};

    for (int i = 0; i < 4; ++i) {
        DinoColor col = (s_selection == i) ? DinoColor_WHITE : DinoColor_GREY;
        std::string label = options[i];

        if (i == 1) {
            label = (s_selection == i) ? "<  " + label + "  >" : label;
        }
        else {
            label = (s_selection == i) ? "> " + label + " <" : label;
        }

        std::vector<DinoVertex> vs;
        DinoVec2 size = Dino_GenVertices_Text(vs, label, col, DinoColor_BLACK);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "PauseOpt");

        float tx = (480.0f - size.x * 2) / 2;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, 140.0f + (i * 30)}, 2);
    }
}