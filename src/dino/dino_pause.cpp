#include "dino_pause.h"
#include <format>
#include <algorithm> // Pour std::max

int DinoPause::s_selection = 0;
double DinoPause::s_lastNavTime = 0;

void DinoPause::Update(double timeSinceStart, bool& bPause, bool& bLobby, double& chrono, double openTime)
{
    DinoGamepad kb, gp;
    bool hasKb = XDino_GetGamepad(DinoGamepadIdx::Keyboard, kb);
    bool hasGp = XDino_GetGamepad(static_cast<DinoGamepadIdx>(0), gp);

    // 1. NAVIGATION (Flèches Haut/Bas)
    if (timeSinceStart > s_lastNavTime + 0.18) {
        bool up = (hasKb && kb.dpad_up) || (hasGp && (gp.dpad_up || gp.stick_left_y < -0.5f));
        bool down = (hasKb && kb.dpad_down) || (hasGp && (gp.dpad_down || gp.stick_left_y > 0.5f));

        if (up) {
            s_selection--;
            s_lastNavTime = timeSinceStart;
        }
        else if (down) {
            s_selection++;
            s_lastNavTime = timeSinceStart;
        }

        if (s_selection < 0) s_selection = 3;
        if (s_selection > 3) s_selection = 0;
    }

    // 2. CHRONO (Le bloc que j'avais oublié !)
    // On ne modifie le chrono que si on est sur la ligne d'index 1
    if (s_selection == 1) {
        if (timeSinceStart > s_lastNavTime + 0.15) {
            bool left = (hasKb && kb.dpad_left) || (hasGp && (gp.dpad_left || gp.stick_left_x < -0.5f));
            bool right = (hasKb && kb.dpad_right) || (hasGp && (gp.dpad_right || gp.stick_left_x > 0.5f));

            if (left) {
                chrono = std::max(0.0, chrono - 10.0);
                s_lastNavTime = timeSinceStart;
            }
            else if (right) {
                chrono += 10.0;
                s_lastNavTime = timeSinceStart;
            }
        }
    }

    // 3. VALIDATION (Touche Entrée / Start)
    bool validate = (hasKb && kb.start) || (hasGp && gp.start);

    if (validate && (timeSinceStart > openTime + 0.2)) {
        if (s_selection == 0) {
            bPause = false;
        }
        else if (s_selection == 2) {
            Dino_BackToLobby();
            bPause = false;
        }
        else if (s_selection == 3) {
            Dino_RestartGame();
            bPause = false;
        }
    }
}

void DinoPause::Draw()
{
    std::string options[] = {"Reprendre", "Chrono +/-", "Retour au Lobby", "Recommencer"};

    for (int i = 0; i < 4; ++i) {
        DinoColor col = (s_selection == i) ? DinoColor_WHITE : DinoColor_GREY;
        std::string label = options[i];

        // Affichage spécial pour la ligne Chrono
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