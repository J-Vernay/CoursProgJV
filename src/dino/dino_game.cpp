/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
std::vector<DinoVec2> polyline;


struct DinoPlayer {
    DinoVec2 playerPos = {};
    bool g_bMiror = false;
    bool isIdle;
    bool isWalking;
    bool isRunning;
};

DinoPlayer player;


// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    player.playerPos = {windowSize.x / 2, windowSize.y / 2};

    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
}

void UpdatePlayer(float time)
{
    float deltaTime = static_cast<float>(time - lastTime);
    lastTime = time;
    float speed;
    bool bMiror = player.g_bMiror;
    // Gestion des entrées et mise à jour de la logique de jeu.

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (gamepad.stick_left_x < 0) {
            bMiror = true;
            player.isIdle = false;
            player.isWalking = true;
        }
        else if (gamepad.stick_left_x > 0) {
            bMiror = false;
            player.isIdle = false;
            player.isWalking = true;
        }
        else {
            player.isIdle = true;
            player.isWalking = false;
        }
        if (!bSuccess)
            continue;

        if (gamepad.btn_right && !gamepad.btn_left) {
            speed = CIRCLE_SPEED * 2;
            player.isRunning = true;
        }
        else {
            speed = CIRCLE_SPEED;
            player.isRunning = false;
        }

        player.playerPos.x += gamepad.stick_left_x * speed * deltaTime;
        player.playerPos.y += gamepad.stick_left_y * speed * deltaTime;
        player.g_bMiror = bMiror;
    }
}

void DrawPlayer(float time)
{
    DinoDrawCall drawcall;
    drawcall.textureName = "dinosaurs.png";

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {24, 0};
    DinoVec2 posC = {0, 24};
    DinoVec2 posD = {24, 24};

    int u = 0;
    if (player.isIdle) {
        int idxFrame = int(time * 8) % 4;
        u = 0 + 24 * idxFrame;
    }
    if (player.isWalking) {
        int idxFrame = int(time * 8) % 6;
        u = 96 + 24 * idxFrame;
    }
    if (player.isIdle) {
        int idxFrame = int(time * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    if (player.g_bMiror) {
        drawcall.vertices.emplace_back(posA, 24, 0);
        drawcall.vertices.emplace_back(posB, 0, 0);
        drawcall.vertices.emplace_back(posC, 24, 24);
        drawcall.vertices.emplace_back(posB, 0, 0);
        drawcall.vertices.emplace_back(posC, 24, 24);
        drawcall.vertices.emplace_back(posD, 0, 24);
    }
    else {
        drawcall.vertices.emplace_back(posA, 0, 0);
        drawcall.vertices.emplace_back(posB, 24, 0);
        drawcall.vertices.emplace_back(posC, 0, 24);
        drawcall.vertices.emplace_back(posB, 24, 0);
        drawcall.vertices.emplace_back(posC, 0, 24);
        drawcall.vertices.emplace_back(posD, 24, 24);
    }
    drawcall.translation = player.playerPos;
    drawcall.scale = 2;
    XDino_Draw(drawcall);
}


void Dino_GameFrame(double timeSinceStart)
{
    UpdatePlayer(timeSinceStart);

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

    DrawPlayer(timeSinceStart);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Ajoutez votre `NOM Prénom` en bas à droite de l'écran,
    {
        std::string text = std::format("FARIN Pix", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_RED);
        drawCall.scale = 1;
        drawCall.rotation = 180;
        drawCall.translation = {windowSize.x, windowSize.y};
        XDino_Draw(drawCall);
    }

}

void Dino_GameShut()
{

}