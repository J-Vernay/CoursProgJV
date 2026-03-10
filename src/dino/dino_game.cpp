/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "../../build/DinoPlayer.h"

#include <__msvc_ostream.hpp>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>
#include <iostream>
#include <cmath>

// Variables globales.

double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
uint64_t texID_imageMilieu;

constexpr float DINO_SPEED = 150.f;
constexpr int NUMBER_PLAYERS = 4;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

std::vector<DinoPlayer> players;

// Constantes.

void Dino_GameInit()
{
    //DinoVec2 windowSize = XDino_GetWindowSize();
    DinoVec2 windowSize = {480, 360};
    XDino_SetRenderSize(windowSize);

    for (int i = 0; i < NUMBER_PLAYERS; i++)
    {
        players.emplace_back(DINO_SPEED, i);
    }

    for (DinoPlayer& player : players)
    {
        player.Start();
    }
}

float currentFrameAnim = 0;

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;
    
    for (int i = 0; i < NUMBER_PLAYERS; i++) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad((DinoGamepadIdx)i, gamepad);
        if (!bSuccess)
            continue;

        players[i].Update(deltaTime, gamepad);
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);
    DinoVec2 renderSize = XDino_GetRenderSize();

    // Dessin de la texture centrale qu'on peut bouger.
    DinoVec2 translation = {renderSize.x / 2, renderSize.y / 2};
    double scale = g_scale * std::min(renderSize.x, renderSize.y) / 4;
    XDino_Draw(vbufID_imageMilieu, texID_imageMilieu, translation, scale, g_rotation);

    // Dessin du cercle que l'on peut bouger.
    

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }
    
    {
        std::string text = std::format("BOURRICAUD Antoine");
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "BOURRICAUD Antoine");
        DinoVec2 position = {0, renderSize.y - text.size()};
        XDino_Draw(vbufID, XDino_TEXID_FONT, position, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

#if !XDINO_RELEASE
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
    for (DinoPlayer& player : players) {
        player.ShutDown();
    }
    
    XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyGpuTexture(texID_imageMilieu);
}