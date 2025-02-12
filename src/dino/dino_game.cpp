/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
DinoVec2 playerPos = {};
std::vector<DinoVec2> polyline;

// Constantes.
constexpr float speed = 300.f; // Nombre de pixels parcourus en une seconde.

bool isRight = true;

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

        float playerSpeed = speed;

        if (gamepad.btn_right && !gamepad.btn_left) {
            playerSpeed *= 2.f;
        }

        playerPos.x += gamepad.stick_left_x * playerSpeed * deltaTime;
        playerPos.y += gamepad.stick_left_y * playerSpeed * deltaTime;

        if (gamepad.stick_left_x != 0) {
            isRight = gamepad.stick_left_x > 0;
        }
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
    {
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

    // Dessin du dinosaure que l'on peut bouger.
    {
        DinoDrawCall drawCall;
        drawCall.textureName = "dinosaurs.png"; // Ici on change en dinosaurs pour avoir accès au sprite sheet. 
        drawCall.vertices.reserve(6);

        DinoVec2 posA = {0, 0};
        DinoVec2 posB = {24, 0};
        DinoVec2 posC = {0, 24};
        DinoVec2 posD = {24, 24};
        if (isRight) {
            drawCall.vertices.emplace_back(posA, 0, 0); // J'ai mis le premier dinosaure bleu
            drawCall.vertices.emplace_back(posB, 24, 0);
            drawCall.vertices.emplace_back(posC, 0, 24);
            drawCall.vertices.emplace_back(posB, 24, 0);
            drawCall.vertices.emplace_back(posC, 0, 24);
            drawCall.vertices.emplace_back(posD, 24, 24);
        }
        else {
            drawCall.vertices.emplace_back(posA, 24, 0);
            drawCall.vertices.emplace_back(posB, 0, 0);
            drawCall.vertices.emplace_back(posC, 24, 24);
            drawCall.vertices.emplace_back(posB, 0, 0);
            drawCall.vertices.emplace_back(posC, 24, 24);
            drawCall.vertices.emplace_back(posD, 0, 24);
        }

        drawCall.scale = 3;
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
    // NOM Prénom
    {
        std::string text = "DUFOUR Enzo";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        drawCall.translation.x = windowSize.x - 2 * textSize.x;
        drawCall.translation.y = windowSize.y - 2 * textSize.y;
        XDino_Draw(drawCall);
    }

}

void Dino_GameShut()
{

}