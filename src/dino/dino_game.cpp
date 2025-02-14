/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "AnimalPool.h"
#include "DinoPlayer.h"
#include "PolyLine.h"
#include "TileMap.h"
#include "UIFrame.h"
#include "UIName.h"

#include <dino/xdino.h>

#include <format>
#include <iostream>
#include <ostream>

// Variables globales.
double lastTime = 0;

//uis
UIName un;
auto uf = UIFrame();

//gameplay
auto pl = PolyLine(4);
auto dp = DinoPlayer();
auto tm = TileMap(2, 16, 12);
auto ap = AnimalPool(10);

//Consts
constexpr DinoColor CLEAR_COLOR = {{50, 50, 80, 255}};
void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    un = UIName("STRAPPAZZON Clement", windowSize);
    dp.Init();
    tm.Init();
    ap.Init();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    tm.Update(deltaTime);
    tm.Draw();
    
    pl.Update(deltaTime);
    pl.Draw();

    ap.Update(deltaTime);
    ap.Draw();
    
    dp.Update(deltaTime);
    dp.Draw();
    
    XDino_SetClearColor(CLEAR_COLOR);
    
    //UIs
    uf.Update(deltaTime);
    uf.Draw();
    un.Update(deltaTime);
    un.Draw();
}

void Dino_GameShut()
{
    //au moins qu'il y ait un truc dedans
    std::cout << "Dino_GameShut" << '\n';
}