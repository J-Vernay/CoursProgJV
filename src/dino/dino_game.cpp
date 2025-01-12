/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_player.h>

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <vector>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> dinos;

void Dino_GameInit()
{
    DinoVec2 rdrSize = XDino_GetWindowSize();

    dinos.emplace_back().Init({rdrSize.x * 0.25f, rdrSize.y * 0.25f}, DinoGamepadIdx::Keyboard, 0);
    dinos.emplace_back().Init({rdrSize.x * 0.75f, rdrSize.y * 0.25f}, DinoGamepadIdx::Gamepad1, 1);
    dinos.emplace_back().Init({rdrSize.x * 0.25f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad2, 2);
    dinos.emplace_back().Init({rdrSize.x * 0.75f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad3, 3);
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Mise à jour

    for (DinoPlayer& dino : dinos)
        dino.Update(timeSinceStart, deltaTime);

    // Affichage

    DinoVec2 rdrSize = XDino_GetWindowSize();

    DinoDrawCall bg;
    bg.textureName = "terrain.png";
    bg.vertices.emplace_back(DinoVec2{0, 0}, 16, 0, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{rdrSize.x, 0}, 32, 0, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{0, rdrSize.y}, 16, 16, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{rdrSize.x, 0}, 32, 0, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{0, rdrSize.y}, 16, 16, DinoColor_WHITE);
    bg.vertices.emplace_back(rdrSize, 32, 16, DinoColor_WHITE);
    XDino_Draw(bg);

    for (DinoPlayer& dino : dinos)
        dino.Draw(timeSinceStart, deltaTime);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}