/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
DinoVec2 circlePos = {};

DinoDrawCall drawPolyline;
DinoDrawCall drawImageMilieu;
DinoDrawCall drawCircle;

// Variable globale pour l'affichage de debug.
int debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    circlePos = {windowSize.x / 2, windowSize.y / 2};

    // Préparation du drawcall de la polyline (zigzag en fond).
    {
        constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

        std::vector<DinoVec2> polyline;
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
        drawPolyline = Dino_CreateDrawCall_Polyline(polyline, 100, POLYLINE_COLOR);
    }

    // Préparation du drawcall de l'image au milieu qu'on peut tourner.
    {
        constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {-1, -1};
        vs[0].color = PURPLE;
        vs[1].pos = {1, -1};
        vs[1].color = CYAN;
        vs[2].pos = {-1, 1};
        vs[2].color = PINK;
        vs[3].pos = {1, -1};
        vs[3].color = CYAN;
        vs[4].pos = {-1, 1};
        vs[4].color = PINK;
        vs[5].pos = {1, 1};
        vs[5].color = ORANGE;
        vs[0].u = 0;
        vs[0].v = 0;
        vs[1].u = 96;
        vs[1].v = 0;
        vs[2].u = 0;
        vs[2].v = 96;
        vs[3].u = 96;
        vs[3].v = 0;
        vs[4].u = 0;
        vs[4].v = 96;
        vs[5].u = 96;
        vs[5].v = 96;

        drawImageMilieu.vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageMilieu");
        drawImageMilieu.texID = XDino_TEXID_FONT;
    }

    // Préparation du drawcall du cercle qu'on peut bouger.
    drawCircle = Dino_CreateDrawCall_Circle(20);
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

        if (gamepad.btn_down && !gamepad.btn_up)
            scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            rotation -= 90.0 * deltaTime;

        circlePos.x += gamepad.stick_left_x * CIRCLE_SPEED * deltaTime;
        circlePos.y += gamepad.stick_left_y * CIRCLE_SPEED * deltaTime;
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    XDino_Draw(drawPolyline);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);
    DinoVec2 renderSize = XDino_GetRenderSize();

    // Dessin de la texture centrale qu'on peut bouger.
    {
        drawImageMilieu.translation = {renderSize.x / 2, renderSize.y / 2};
        drawImageMilieu.rotation = rotation;
        drawImageMilieu.scale = scale * std::min(renderSize.x, renderSize.y) / 4;
        XDino_Draw(drawImageMilieu);
    }

    // Dessin du cercle que l'on peut bouger.
    {
        drawCircle.translation = circlePos;
        XDino_Draw(drawCircle);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
        XDino_DestroyVertexBuffer(drawCall.vbufID);
    }

    // Affichage des statistiques si on appuie sur SHIFT.
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.select) {
        int diff = 0;
        if (keyboard.dpad_up)
            diff -= 1;
        if (keyboard.dpad_down)
            diff += 1;
        debugScroll += diff;
        debugScroll = XDino_DrawStats(debugScroll);
    }
}

void Dino_GameShut()
{
    XDino_DestroyVertexBuffer(drawCircle.vbufID);
    XDino_DestroyVertexBuffer(drawImageMilieu.vbufID);
    XDino_DestroyVertexBuffer(drawPolyline.vbufID);
}