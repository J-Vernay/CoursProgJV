/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
//DinoVec2 circlePos = {};
DinoVec2 playerPos = {};
std::vector<DinoVec2> polyline;
bool goingRight;

// Constantes.
constexpr float Dino_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    playerPos = {windowSize.x / 2, windowSize.y / 2};

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

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        /*if (gamepad.btn_down && !gamepad.btn_up)
            scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            rotation -= 90.0 * deltaTime;*/

        float speed = Dino_SPEED;
        if (gamepad.btn_right)
            speed *= 2;

        playerPos.x += gamepad.stick_left_x * speed * deltaTime;
        playerPos.y += gamepad.stick_left_y * speed * deltaTime;
        goingRight = gamepad.stick_left_x >= 0;
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne" 
    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(polyline, 100, POLYLINE_COLOR);
        XDino_Draw(drawCall);
    }

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    // Dessin de la texture centrale qu'on peut bouger.
    /*{
        constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

        float quarterWidth = windowSize.x / 4;
        float quarterHeight = windowSize.y / 4;

        DinoDrawCall drawCall;
        drawCall.vertices.resize(6);
        drawCall.vertices[0].pos = {-quarterWidth, -quarterHeight};
        drawCall.vertices[0].color = PURPLE;
        drawCall.vertices[1].pos = {quarterWidth, -quarterHeight};
        drawCall.vertices[1].color = CYAN;
        drawCall.vertices[2].pos = {-quarterWidth, quarterHeight};
        drawCall.vertices[2].color = PINK;
        drawCall.vertices[3].pos = {quarterWidth, -quarterHeight};
        drawCall.vertices[3].color = CYAN;
        drawCall.vertices[4].pos = {-quarterWidth, quarterHeight};
        drawCall.vertices[4].color = PINK;
        drawCall.vertices[5].pos = {quarterWidth, quarterHeight};
        drawCall.vertices[5].color = ORANGE;

        drawCall.translation = {windowSize.x / 2, windowSize.y / 2};
        drawCall.rotation = rotation;
        drawCall.scale = scale;

        drawCall.vertices[0].u = 0;
        drawCall.vertices[0].v = 0;
        drawCall.vertices[1].u = 96;
        drawCall.vertices[1].v = 0;
        drawCall.vertices[2].u = 0;
        drawCall.vertices[2].v = 96;
        drawCall.vertices[3].u = 96;
        drawCall.vertices[3].v = 0;
        drawCall.vertices[4].u = 0;
        drawCall.vertices[4].v = 96;
        drawCall.vertices[5].u = 96;
        drawCall.vertices[5].v = 96;
        drawCall.textureName = "monogram-bitmap.png";

        XDino_Draw(drawCall);
    }

    // Dessin du cercle que l'on peut bouger.
    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Circle(20);
        drawCall.translation = circlePos;
        XDino_Draw(drawCall);
    }*/

    // Dessin du dino
    {
        DinoDrawCall drawCall;
        float radiusX = 24;
        float radiusY = 24;
        drawCall.textureName = "dinosaurs.png";
        drawCall.vertices.reserve(6);
        if (!goingRight)
            radiusX = -radiusX;

        DinoVec2 posA = {-radiusX, -radiusY};
        DinoVec2 posB = {radiusX, -radiusY};
        DinoVec2 posC = {-radiusX, radiusY};
        DinoVec2 posD = {radiusX, radiusY};
        DinoColor color = DinoColor_WHITE;

        drawCall.vertices.emplace_back(posA, 0, 0, color);
        drawCall.vertices.emplace_back(posB, 24, 0, color);
        drawCall.vertices.emplace_back(posC, 0, 24, color);
        drawCall.vertices.emplace_back(posB, 24, 0, color);
        drawCall.vertices.emplace_back(posC, 0, 24, color);
        drawCall.vertices.emplace_back(posD, 24, 24, color);
        drawCall.translation = playerPos;
        XDino_Draw(drawCall);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
    // nom et prénom
    {
        std::string text = "Pharrell Bastien";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        drawCall.translation = {windowSize.x - textSize.x * 2, windowSize.y - textSize.y * 2};

        XDino_Draw(drawCall);
    }

}

void Dino_GameShut()
{

}