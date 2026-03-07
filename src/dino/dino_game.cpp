/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;
DinoVec2 g_circlePos = {};

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
uint64_t vbufID_circle;
uint64_t texID_imageMilieu;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_circlePos = {windowSize.x / 2, windowSize.y / 2};

    // Préparation du drawcall de la polyline (zigzag en fond).
    {
        constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

        std::vector<DinoVec2> polyline;
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Polyline(vs, polyline, 100, POLYLINE_COLOR);
        vbufID_polyline = XDino_CreateVertexBuffer(vs, "Polyline");
    }

    // Préparation du drawcall de l'image au milieu qu'on peut tourner.
    {
        constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

        texID_imageMilieu = XDino_CreateGpuTexture("animals.png");
        DinoVec2 texSize = XDino_GetGpuTextureSize(texID_imageMilieu);

        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {-2, -1};
        vs[0].color = PURPLE;
        vs[1].pos = {2, -1};
        vs[1].color = CYAN;
        vs[2].pos = {-2, 1};
        vs[2].color = PINK;
        vs[3].pos = {2, -1};
        vs[3].color = CYAN;
        vs[4].pos = {-2, 1};
        vs[4].color = PINK;
        vs[5].pos = {2, 1};
        vs[5].color = ORANGE;
        vs[0].u = 0;
        vs[0].v = 0;
        vs[1].u = texSize.x;
        vs[1].v = 0;
        vs[2].u = 0;
        vs[2].v = texSize.y;
        vs[3].u = texSize.x;
        vs[3].v = 0;
        vs[4].u = 0;
        vs[4].v = texSize.y;
        vs[5].u = texSize.x;
        vs[5].v = texSize.y;

        vbufID_imageMilieu = XDino_CreateVertexBuffer(vs, "ImageMilieu");
    }

    // Préparation du drawcall du cercle qu'on peut bouger.
    {
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Circle(vs, 20);
        vbufID_circle = XDino_CreateVertexBuffer(vs, "Circle");
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.btn_down && !gamepad.btn_up)
            g_scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            g_scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            g_rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            g_rotation -= 90.0 * deltaTime;

        g_circlePos.x += gamepad.stick_left_x * CIRCLE_SPEED * deltaTime;
        g_circlePos.y += gamepad.stick_left_y * CIRCLE_SPEED * deltaTime;
    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);
    DinoVec2 renderSize = XDino_GetRenderSize();

    // Dessin de la texture centrale qu'on peut bouger.
    DinoVec2 translation = {renderSize.x / 2, renderSize.y / 2};
    double scale = g_scale * std::min(renderSize.x, renderSize.y) / 4;
    XDino_Draw(vbufID_imageMilieu, texID_imageMilieu, translation, scale, g_rotation);

    // Dessin du cercle que l'on peut bouger.
    XDino_Draw(vbufID_circle, XDino_TEXID_FONT, g_circlePos);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs, "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
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
        XDino_DrawStats(diff);
    }
}

void Dino_GameShut()
{
    XDino_DestroyVertexBuffer(vbufID_circle);
    XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyGpuTexture(texID_imageMilieu);
}