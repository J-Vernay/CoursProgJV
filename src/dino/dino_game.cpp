/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "Dino.h"

#include <algorithm>
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>
#include <iostream>
#include <ostream>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
DinoVec2 circlePos = {};
std::vector<DinoVec2> polyline;
Dino d("dinosaurs.png", 0, {0, 0});
std::vector<Dino> dinos;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    circlePos = {windowSize.x / 2, windowSize.y / 2};
    
    dinos.resize(4);
    dinos[0] = d;
    dinos[1] = Dino("dinosaurs.png", 1);
    dinos[2] = Dino("dinosaurs.png", 2);
    dinos[3] = Dino("dinosaurs.png", 3);
    
    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.
    int i = 0;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.btn_down && !gamepad.btn_up)
            scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            rotation -= 90.0 * deltaTime;

        dinos[i].Update(deltaTime, gamepad);
        i++;
    }
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {{50, 50, 80, 255}};
    constexpr DinoColor POLYLINE_COLOR = {{70, 70, 100, 255}};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne" 
    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(polyline, 100, POLYLINE_COLOR);
        XDino_Draw(drawCall);
    }

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
    
    {
        std::string t = std::format("Strappazzon Clement");
        DinoVec2 size;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(t, DinoColor_WHITE, DinoColor_ORANGE, &size);
        drawCall.scale = 2;
        drawCall.translation = {windowSize.x - size.x * 2, windowSize.y - size.y * 2};
        XDino_Draw(drawCall);
    }

    {
        std::ranges::sort(dinos, Dino::CompareHeight);
        for (Dino di : dinos)
        {
            DinoDrawCall drawCall = Dino_CreateDrawCall_Circle(20);
        
            //Seuls funs a appeler
            di.Draw(&drawCall);
        
            XDino_Draw(drawCall);
        }
    }
}

void Dino_GameShut()
{
    //au moins qu'il y ait un truc dedans
    std::cout << "Dino_GameShut" << '\n';
}