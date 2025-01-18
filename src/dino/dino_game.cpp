/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_player.h>
#include <dino/dino_terrain.h>

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <vector>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> g_Dinos;
DinoTerrain g_Terrain;

void Dino_GameInit()
{
    DinoVec2 rdrSize = {320, 240};
    XDino_SetRenderSize(rdrSize);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.25f, rdrSize.y * 0.25f}, DinoGamepadIdx::Keyboard, 0);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.75f, rdrSize.y * 0.25f}, DinoGamepadIdx::Gamepad1, 1);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.25f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad2, 2);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.75f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad3, 3);

    g_Terrain.Init(16, 12);
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Mise à jour

    for (DinoPlayer& dino : g_Dinos)
        dino.Update(timeSinceStart, deltaTime);

    // Affichage

    g_Terrain.Draw(timeSinceStart, deltaTime);

    DinoDrawCall drawDinos = DinoPlayer::DrawCallDinos(g_Dinos, timeSinceStart, deltaTime);
    XDino_Draw(drawDinos);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}