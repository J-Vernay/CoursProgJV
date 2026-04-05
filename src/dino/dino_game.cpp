/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_tree.h"
#include "game_state.h"

#include <algorithm>
#include <dino/dino_terrain.h>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <dino/dino_score.h>
#include <dino/Dino_GameStates.h>
#include <dino/Lobby_state.h>
#include <dino/Playing_state.h>
#include <iostream>

#include <format>
#include <map>

DinoGameState g_gameState;
double g_lastTime;
int g_debugScroll = 0;


void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    g_gameState.Init(0.0);

}


void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    XDino_SetRenderSize({480, 360});

    DinoVec2 renderSize = XDino_GetRenderSize();

    g_gameState.Update(deltaTime, timeSinceStart);

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur SHIFT.
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.shoulder_left) {
        int diff = 0;
        if (keyboard.dpad_up)
            diff -= 1;
        if (keyboard.dpad_down)
            diff += 1;
        XDino_DrawStats(diff);
    }
#endif
}


void Dino_GameShut()
{
    g_gameState.Shut();
    DinoTree::ShutStatic();
    DinoPlayer::ShutStatic();
}