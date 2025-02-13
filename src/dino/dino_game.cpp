/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "DinoPlayer.h"
#include "PolyLine.h"
#include "UIFrame.h"
#include "UIName.h"

#include <dino/xdino.h>

#include <format>
#include <iostream>
#include <ostream>

// Variables globales.
double lastTime = 0;
auto un = UIName("Strappazzon Clement");
auto uf = UIFrame();
auto pl = PolyLine(4);
auto dp = DinoPlayer();

//Consts
constexpr DinoColor CLEAR_COLOR = {{50, 50, 80, 255}};
void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    dp.Init();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    dp.Update(deltaTime);
    dp.Draw();
    
    XDino_SetClearColor(CLEAR_COLOR);

    pl.Update(deltaTime);
    pl.Draw();
    
    //UIs
    uf.Update(deltaTime);
    uf.Draw(windowSize);
    un.Update(deltaTime);
    un.Draw(windowSize);
}

void Dino_GameShut()
{
    //au moins qu'il y ait un truc dedans
    std::cout << "Dino_GameShut" << '\n';
}