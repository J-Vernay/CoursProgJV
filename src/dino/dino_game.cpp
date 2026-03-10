/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include "dino/dino_player.h"
#include "dino/dino_terrain.h"
#include <format>

// Variables globales.
double g_lastTime = 0;
bool g_prevDown = false;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Objet dinosaure
std::vector<DinoPlayer> dinos(4);
int currentPlayer = 0;

// Terrain
DinoTerrain terrain;

void Dino_GameInit()
{
    /*
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);*/
    DinoVec2 windowSize = {480.f, 360.f};
    XDino_SetRenderSize(windowSize);

    terrain.Init();

    // Couleurs (V) différentes pour chaque dinosaure
    std::vector<int> startV = {0, 24, 48, 72};

    for (int i = 0; i < dinos.size(); ++i) {
        dinos[i].Init();
        dinos[i].currentV = startV[i];
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    bool downPressed = false;

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        if (!XDino_GetGamepad(idx, gamepad))
            continue;

        if (gamepad.btn_down)
            downPressed = true;
    }

    // Détection du moment où le bouton vient d'être pressé
    if (downPressed && !g_prevDown) {
        currentPlayer = (currentPlayer + 1) % dinos.size();
    }

    g_prevDown = downPressed;

    // Update seulement le joueur actif
    dinos[currentPlayer].Update(timeSinceStart, deltaTime);

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);
    terrain.Draw(timeSinceStart);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);
    DinoVec2 renderSize = XDino_GetRenderSize();

    // Draw tous les dinos
    for (DinoPlayer& dino : dinos) {
        dino.Draw(timeSinceStart);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    // Affichage du nom en bas à droite
    {
        std::string myName = "Chauvin Remi";
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, myName, DinoColor_WHITE, DinoColor_GREY);
        DinoVec2 translation = {renderSize.x - textSize.x * 2, renderSize.y - textSize.y * 2};
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "myName");
        XDino_Draw(vbufID, XDino_TEXID_FONT, translation, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

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
    terrain.Shut();
    for (DinoPlayer& dino : dinos) {
        dino.Shut();
    }
}