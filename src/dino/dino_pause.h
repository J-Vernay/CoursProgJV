#pragma once
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <vector>
#include <string>

class DinoPause {
public:
    // On passe les variables par référence pour pouvoir les modifier directement
    static void Update(double timeSinceStart, bool& bPause, bool& bLobby, double& chrono);
    static void Draw();

private:
    static int s_selection;
    static double s_lastNavTime;
};