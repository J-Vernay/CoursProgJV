/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/GameManager/DinoGameState.h>
#include "LassoManager.h"
#include "GameManager/PlayingState.h"

#include <dino/Entities/dino_animal.h>
#include <dino/Entities/dino_player.h>
#include <dino/Entities/dino_EntityManager.h>

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>
#include <unordered_map>

// Variables globales.
double g_lastTime = 0;

DinoGameState g_gameState;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;


void Dino_GameInit()
{
    //DinoVec2 windowSize = XDino_GetWindowSize();
    DinoVec2 windowSize = {480, 360};
    XDino_SetRenderSize(windowSize);

    // Préparation du drawcall du prenom.
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "DEMAGNEE Aloys", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "name");
    }

    g_gameState.Init();
    dino_animal::DinoAnimal_InitStatic();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    DinoVec2 renderSize = XDino_GetRenderSize();

    //drawing terrain
    g_gameState.Update(deltaTime, timeSinceStart);

    
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    //DEBUG ONLY
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, {}, 2);
    }
    {
        DinoVec2 translation = {renderSize.x - textSize_prenom.x * 2, renderSize.y - textSize_prenom.y * 2};
        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, translation, 2);
    }

    

#if !XDINO_RELEASE
    //DEBUG ONLY
    // Affichage des statistiques si on appuie sur CTRL.
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
    g_gameState.Shutdown();
    dino_animal::DinoAnimal_ShutStatic();

    XDino_DestroyVertexBuffer(vbufID_prenom);
}