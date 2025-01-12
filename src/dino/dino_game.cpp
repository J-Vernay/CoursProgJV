/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
DinoVec2 dinoPos;
bool bDinoLeft = false;
bool bDinoDamage = false;
double dinoDamageStart = 0;

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

    // Gestion des entrées.

    bool bDinoRunning = false;
    DinoVec2 bDinoMove{};

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad);
    if (bSuccess) {
        bDinoRunning = gamepad.btn_right;
        bDinoMove.x = gamepad.stick_left_x;
        bDinoMove.y = gamepad.stick_left_y;
        if (gamepad.shoulder_left) {
            bDinoDamage = true;
            dinoDamageStart = timeSinceStart;
        }
    }

    // Logique de jeu.

    if (bDinoDamage && timeSinceStart - dinoDamageStart > 3)
        bDinoDamage = false;

    float speed = 200;
    if (bDinoRunning)
        speed *= 2;
    if (bDinoDamage)
        speed = 0;

    dinoPos.x += bDinoMove.x * speed * deltaTime;
    dinoPos.y += bDinoMove.y * speed * deltaTime;
    if (bDinoMove.x < 0)
        bDinoLeft = true;
    if (bDinoMove.x > 0)
        bDinoLeft = false;

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
    dino.vertices.emplace_back(DinoVec2{-12, -12}, 0, 0, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{+12, +12}, 24, 24, DinoColor_WHITE);
    // Mode miroir si on va à gauche.
    if (bDinoLeft) {
        for (DinoVertex& vertex : dino.vertices)
            vertex.u = 24 - vertex.u;
    }

    // Animation
    if (bDinoDamage) {
        // Animation de dégât
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 3;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(336 + 24 * idxFrame);
    }
    else if (bDinoMove.x == 0 && bDinoMove.y == 0) {
        // Animation sur place
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 4;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(24 * idxFrame);
    }
    else if (bDinoRunning) {
        // Animation de course
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 16) % 6;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(432 + 24 * idxFrame);

    }
    else {
        // Animation de marche
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 6;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(96 + 24 * idxFrame);
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