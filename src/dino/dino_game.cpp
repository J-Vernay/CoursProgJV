/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double scale = 1.0;

struct DinoPlayer {
    DinoVec2 pos = {};
    bool bMirror = false;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;
};

DinoPlayer g_Player;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void UpdatePlayer(float deltaTime)
{
    g_Player.bIdle = false;
    g_Player.bRunning = false;
    g_Player.bWalking = false;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        float speed = CIRCLE_SPEED;
        if (gamepad.btn_right) {
            speed *= 2;
        }

        g_Player.pos.x += gamepad.stick_left_x * speed * deltaTime;
        g_Player.pos.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x != 0) {
            g_Player.bMirror = gamepad.stick_left_x < 0;
        }

        g_Player.bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
        if (!g_Player.bIdle) {
            if (gamepad.btn_right)
                g_Player.bRunning = true;
            else
                g_Player.bWalking = true;
        }
    }
}

void DrawDino(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};

    int u = 0;
    if (g_Player.bIdle) {
        int idxFrame = static_cast<int>(timeSinceStart * 8) % 4;
        u = 0 + 24 * idxFrame;
    }

    if (g_Player.bWalking) {
        int idxFrame = static_cast<int>(timeSinceStart * 8) % 6;
        u = 96 + 24 * idxFrame;
    }

    if (g_Player.bRunning) {
        int idxFrame = static_cast<int>(timeSinceStart * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    if (g_Player.bMirror) {
        drawCall.vertices.emplace_back(posA, u + 24, 0);
        drawCall.vertices.emplace_back(posB, u + 0, 0);
        drawCall.vertices.emplace_back(posC, u + 24, 24);
        drawCall.vertices.emplace_back(posB, u + 0, 0);
        drawCall.vertices.emplace_back(posC, u + 24, 24);
        drawCall.vertices.emplace_back(posD, u + 0, 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, 0);
        drawCall.vertices.emplace_back(posB, u + 24, 0);
        drawCall.vertices.emplace_back(posC, u + 0, 24);
        drawCall.vertices.emplace_back(posB, u + 24, 0);
        drawCall.vertices.emplace_back(posC, u + 0, 24);
        drawCall.vertices.emplace_back(posD, u + 24, 24);
    }

    drawCall.scale = 3;
    drawCall.translation = g_Player.pos;
    XDino_Draw(drawCall);
}

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_Player.pos = {windowSize.x / 2, windowSize.y / 2};
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    UpdatePlayer(deltaTime);

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    DrawDino(timeSinceStart);

    // Clément
    {
        std::string text = std::format("LI Clement");
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.scale = 2;
        // translation en bas à droite
        drawCall.translation = {windowSize.x - textSize.x * 2, windowSize.y - textSize.y * 2};
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}