/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
DinoVec2 dinoPos;
bool bDinoLeft = false;

void Dino_GameInit()
{
    DinoVec2 rdrSize = XDino_GetWindowSize();
    dinoPos = {rdrSize.x / 2, rdrSize.y / 2};
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad);
    if (bSuccess) {
        float speed = 300;
        if (gamepad.btn_right)
            speed *= 2;
        dinoPos.x += gamepad.stick_left_x * speed * deltaTime;
        dinoPos.y += gamepad.stick_left_y * speed * deltaTime;
        if (gamepad.stick_left_x < 0)
            bDinoLeft = true;
        if (gamepad.stick_left_x > 0)
            bDinoLeft = false;
    }

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

    DinoDrawCall dino;
    dino.textureName = "dinosaurs.png";
    if (bDinoLeft) {
        dino.vertices.emplace_back(DinoVec2{-12, -12}, 24, 0, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{+12, -12}, 0, 0, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{-12, +12}, 24, 24, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{+12, -12}, 0, 0, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{-12, +12}, 24, 24, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{+12, +12}, 0, 24, DinoColor_WHITE);
    }
    else {
        dino.vertices.emplace_back(DinoVec2{-12, -12}, 0, 0, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
        dino.vertices.emplace_back(DinoVec2{+12, +12}, 24, 24, DinoColor_WHITE);
    }
    dino.scale = 2;
    dino.translation = dinoPos;
    XDino_Draw(dino);

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