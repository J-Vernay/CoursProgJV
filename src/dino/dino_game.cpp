/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;

struct DinoPlayer {
    DinoVec2 pos = {};
    bool bMirror = false;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;
    int idxPlayer = 0;

    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);
};

DinoPlayer g_Player1;
DinoPlayer g_Player2;
DinoPlayer g_Player3;
DinoPlayer g_Player4;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void DinoPlayer::UpdatePlayer(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    
    this->bIdle = false;
    this->bWalking = false;
    this->bRunning = false;
    
	// Copier coller du code
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        float speed = CIRCLE_SPEED;
        if (gamepad.btn_right)
            speed *= 2;
        
        this->pos.x += gamepad.stick_left_x * speed * deltaTime;
        this->pos.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x != 0)
            this->bMirror = gamepad.stick_left_x < 0;

        this->bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
        if (!this->bIdle) {
            if (gamepad.btn_right)
                this->bRunning = true;
            else
                this->bWalking = true;
        }
    }
}

void DinoPlayer::DrawPlayer(double timeSinceStart)
{
    // Copier-coller de l'ancien code.
    
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};

    int u = 0;
    if (this->bIdle) {
        int idxFrame = int(timeSinceStart * 8) % 4;
        u = 0 + 24 * idxFrame;
    }
    if (this->bWalking) {
        int idxFrame = int(timeSinceStart * 8) % 6;
        u = 96 + 24 * idxFrame;
    }
    if (this->bRunning) {
        int idxFrame = int(timeSinceStart * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    int v = idxPlayer * 24; // 24 pixels par ligne de sprite

    if (this->bMirror) {
        drawCall.vertices.emplace_back(posA, u + 24, v + 0);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posD, u + 0, v + 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, v + 0);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posD, u + 24, v + 24);
    }
    
    drawCall.scale = 3;
    drawCall.translation = this->pos;
    XDino_Draw(drawCall);
}

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_Player1.pos = {windowSize.x / 2, windowSize.y / 2};
    g_Player1.idxPlayer = 0;
    g_Player2.pos = {windowSize.x / 2 + 100, windowSize.y / 2};
    g_Player2.idxPlayer = 1;
    g_Player3.pos = {windowSize.x / 2, windowSize.y / 2 + 100};
    g_Player3.idxPlayer = 2;
    g_Player4.pos = {windowSize.x / 2 + 100, windowSize.y / 2 + 100};
    g_Player4.idxPlayer = 3;
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    g_Player1.UpdatePlayer(deltaTime);
    g_Player2.UpdatePlayer(deltaTime);
    g_Player3.UpdatePlayer(deltaTime);
    g_Player4.UpdatePlayer(deltaTime);
    
    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    g_Player1.DrawPlayer(timeSinceStart);
    g_Player2.DrawPlayer(timeSinceStart);
    g_Player3.DrawPlayer(timeSinceStart);
    g_Player4.DrawPlayer(timeSinceStart);
    
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Prénom.
    {
        std::string text = "Julien Vernay";
        DinoVec2 textSize;
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
        drawCall.translation.x = windowSize.x - 2 * textSize.x;
        drawCall.translation.y = windowSize.y - 2 * textSize.y;
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}