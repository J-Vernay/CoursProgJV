/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include "dino/dino_player.h"
#include "dino/dino_terrain.h"
#include "dino/dino_animal.h"
#include <map>
#include <format>
#include <iostream>

// Variables globales.
std::map<int, int> controllerToDino;
double g_lastTime = 0;

// Objet dinosaure
std::vector<DinoPlayer> dinos(4);

// Terrain
DinoTerrain terrain;

// Animaux
DinoAnimals animals;

void Dino_GameInit()
{
    /*
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);*/
    DinoVec2 windowSize = {480.f, 360.f};
    XDino_SetRenderSize(windowSize);

    // Initialiser le terrain
    terrain.Init();

    // Initialiser les dinosaures
    std::vector<int> startV = {0, 24, 48, 72};
    for (int i = 0; i < dinos.size(); ++i) {
        dinos[i].Init(terrain, startV[i]);
    }

    // Assigner chaque contrôleur à un dino
    int assigned = 0;
    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL) {
        DinoGamepad gp;
        if (XDino_GetGamepad(idx, gp) && assigned < dinos.size()) {
            controllerToDino[static_cast<int>(idx)] = assigned;
            std::cout << "Controller " << static_cast<int>(idx)
                << " assigned to Dino " << assigned << "\n";
            ++assigned;
        }
    }

    // Initialiser les animaux
    animals.Init();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Update tous les dinosaures avec leur contrôleur assigné
    for (const auto& [controllerIdx, dinoIdx] : controllerToDino) {
        DinoGamepad gp{};
        if (!XDino_GetGamepad(static_cast<DinoGamepadIdx>(controllerIdx), gp))
            continue;

        dinos[dinoIdx].Update(timeSinceStart, deltaTime, terrain, gp);
    }

    // Spawn et update des animaux
    animals.TrySpawn(timeSinceStart, terrain);
    animals.Update(timeSinceStart, deltaTime, terrain);

    // Collision entre tous les dinos
    for (int i = 0; i < dinos.size(); ++i) {
        for (int j = i + 1; j < dinos.size(); ++j) {
            dinos[i].RepulseWith(dinos[j]);
        }
    }

    // Collision entre animaux
    animals.RepulseAnimals();

    // Collision entre animaux et dinos

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);
    terrain.Draw(timeSinceStart);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);
    DinoVec2 renderSize = XDino_GetRenderSize();

    // Draw tous les animaux
    animals.Draw(timeSinceStart);
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
    animals.Shut();
}