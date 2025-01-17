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
    DinoVec2 rdrSize = {320, 240};
    XDino_SetRenderSize(rdrSize);
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

    DinoVec2 rdrSize = XDino_GetRenderSize();

    constexpr int32_t tileCountX = 16;
    constexpr int32_t tileCountY = 12;

    // Océan, draw call séparé car il doit représenter tout l'écran, alors que le terrain peut être translaté.
    DinoDrawCall bg;
    bg.textureName = "terrain.png";
    bg.vertices.emplace_back(DinoVec2{0, 0}, 0, 0, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{rdrSize.x, 0}, 16, 0, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{0, rdrSize.y}, 0, 16, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{rdrSize.x, 0}, 16, 0, DinoColor_WHITE);
    bg.vertices.emplace_back(DinoVec2{0, rdrSize.y}, 0, 16, DinoColor_WHITE);
    bg.vertices.emplace_back(rdrSize, 16, 16, DinoColor_WHITE);
    XDino_Draw(bg);

    DinoDrawCall terrain;
    terrain.textureName = "terrain.png";

    // Coins
    Dino_AddDraw_Rect(terrain, {0, 0}, {16, 16}, {0, 16});
    Dino_AddDraw_Rect(terrain, {16 * (tileCountX - 1), 0}, {16, 16}, {32, 16});
    Dino_AddDraw_Rect(terrain, {0, 16 * (tileCountY - 1)}, {16, 16}, {0, 48});
    Dino_AddDraw_Rect(terrain, {16 * (tileCountX - 1), 16 * (tileCountY - 1)}, {16, 16}, {32, 48});

    // Frontières
    for (int32_t i = 1; i < tileCountX - 1; ++i) {
        Dino_AddDraw_Rect(terrain, {16.f * i, 0}, {16, 16}, {16, 16});
        Dino_AddDraw_Rect(terrain, {16.f * i, 16 * (tileCountY - 1)}, {16, 16}, {16, 48});
    }
    for (int32_t i = 1; i < tileCountY - 1; ++i) {
        Dino_AddDraw_Rect(terrain, {0, 16.f * i}, {16, 16}, {0, 32});
        Dino_AddDraw_Rect(terrain, {16 * (tileCountX - 1), 16.f * i}, {16, 16}, {32, 32});
    }

    // Milieu
    terrain.vertices.emplace_back(DinoVec2{16, 16}, 16, 0, DinoColor_WHITE);
    terrain.vertices.emplace_back(DinoVec2{16 * (tileCountX - 1), 16}, 32, 0, DinoColor_WHITE);
    terrain.vertices.emplace_back(DinoVec2{16, 16 * (tileCountY - 1)}, 16, 16, DinoColor_WHITE);
    terrain.vertices.emplace_back(DinoVec2{16 * (tileCountX - 1), 16}, 32, 0, DinoColor_WHITE);
    terrain.vertices.emplace_back(DinoVec2{16, 16 * (tileCountY - 1)}, 16, 16, DinoColor_WHITE);
    terrain.vertices.emplace_back(DinoVec2{16 * (tileCountX - 1), 16 * (tileCountY - 1)}, 32, 16, DinoColor_WHITE);

    // Grille centrée
    terrain.translation.x = (rdrSize.x - (tileCountX * 16)) / 2;
    terrain.translation.y = (rdrSize.y - (tileCountY * 16)) / 2;

    XDino_Draw(terrain);

    DinoDrawCall drawDinos = DinoPlayer::DrawCallDinos(dinos, timeSinceStart, deltaTime);
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