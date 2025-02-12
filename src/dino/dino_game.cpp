/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
DinoVec2 dinoPos = {};
std::vector<DinoVec2> polyline;

// Constantes.
constexpr float speed = 300.f; // Nombre de pixels parcourus en une seconde.

struct DinoPlayer {
    DinoVec2 pos;
    bool bMirror;
    bool bIdle;
    bool bWalking;
    bool bRunning;

    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);
};

DinoPlayer g_player;

void DinoPlayer::UpdatePlayer(float deltaTime)
{

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.btn_down && !gamepad.btn_up)
            scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            rotation -= 90.0 * deltaTime;

        dinoPos.x += gamepad.stick_left_x * speed * deltaTime;
        dinoPos.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x != 0) {
            g_player.bMirror = gamepad.stick_left_x < 0;
        }

        bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;

        if (!bIdle) {
            if (gamepad.btn_right) {
                bRunning = true;
                bWalking = false;
            }

            else {
                bRunning = false;
                bWalking = true;
            }
        }
        else {
            bRunning = false;
            bWalking = false;
        }
    }
}

void DinoPlayer::DrawPlayer(double timeSinceStart)
{

    // Dessin du dinosaur qu'on peut bouger
    {
        DinoDrawCall drawCall;
        drawCall.textureName = "dinosaurs.png";
        DinoVec2 posA = {0, 0};
        DinoVec2 posB = {24, 0};
        DinoVec2 posC = {0, 24};
        DinoVec2 posD = {24, 24};

        int u = 0;

        if (bIdle) {
            int idxFrame = int(timeSinceStart * 8) % 4;
            u = 0 + 24 * idxFrame;
        }
        if (bWalking) {
            int idxFrame = int(timeSinceStart * 8) % 6;
            u = 96 + 24 * idxFrame;
        }
        if (bRunning) {
            int idxFrame = int(timeSinceStart * 16) % 6;
            u = 432 + 24 * idxFrame;
        }

        switch (g_player.bMirror) {
        case false: drawCall.vertices.emplace_back(posA, 0 + u, 0);
            drawCall.vertices.emplace_back(posB, 24 + u, 0);
            drawCall.vertices.emplace_back(posC, 0 + u, 24);
            drawCall.vertices.emplace_back(posB, 24 + u, 0);
            drawCall.vertices.emplace_back(posC, 0 + u, 24);
            drawCall.vertices.emplace_back(posD, 24 + u, 24);
            break;
        case true: drawCall.vertices.emplace_back(posA, 24 + u, 0);
            drawCall.vertices.emplace_back(posB, 0 + u, 0);
            drawCall.vertices.emplace_back(posC, 24 + u, 24);
            drawCall.vertices.emplace_back(posB, 0 + u, 0);
            drawCall.vertices.emplace_back(posC, 24 + u, 24);
            drawCall.vertices.emplace_back(posD, 0 + u, 24);
            break;
        }

        drawCall.scale = scale;
        drawCall.translation = dinoPos;

        XDino_Draw(drawCall);

    }
}

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_player.pos = {windowSize.x / 2, windowSize.y / 2};
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    g_player.UpdatePlayer(deltaTime);

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    g_player.DrawPlayer(timeSinceStart);

    /*// Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
{
    std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
    drawCall.scale = 2;
    XDino_Draw(drawCall);
}

//Prénom
{
    std::string text = "Thomas Druesne";
    DinoVec2 textSize;

    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
    drawCall.scale = 2;
    // Placer en bas a droite dans le coin en utilisant le parametre de retour poutsize

    drawCall.translation.x = windowSize.x - 2 * textSize.x;
    drawCall.translation.y = windowSize.y - 2 * textSize.y;

    // Afficher le texte
    XDino_Draw(drawCall);
}*/

}


void Dino_GameShut()
{

}


/*// Dessin de la texture centrale qu'on peut bouger.
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
    }*/