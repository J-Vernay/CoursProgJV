/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

struct DinoPlayer {
    DinoVec2 pos = {};
    bool bLeft = false;
    double endHitAnim = 0;
};

DinoPlayer g_Player;

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
uint64_t texID_imageMilieu;
uint64_t texID_dino;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

uint64_t DinoPlayer_GenerateVertexBuffer(DinoPlayer& player, double timeSinceStart, bool bMoving, bool bPressedRun)
{
    float animSpeed;
    int frameCount;
    int ubase;
    if (timeSinceStart < player.endHitAnim) {
        // ANIM HIT
        animSpeed = 8;
        frameCount = 3;
        ubase = 336;
    }
    else if (bMoving) {
        if (bPressedRun) {
            // ANIM RUN
            animSpeed = 16;
            frameCount = 6;
            ubase = 432;
        }
        else {
            // ANIM WALK
            animSpeed = 8;
            frameCount = 6;
            ubase = 96;
        }
    }
    else {
        // ANIM IDLE
        animSpeed = 8;
        frameCount = 4;
        ubase = 0;
    }

    int uAnim = ((int)(timeSinceStart * animSpeed) % frameCount) * 24 + ubase;

    std::vector<DinoVertex> vs;
    uint16_t umin, umax;
    if (player.bLeft) {
        umin = uAnim + 24;
        umax = uAnim + 0;
    }
    else {
        umin = uAnim + 0;
        umax = uAnim + 24;
    }

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin;
    vs[0].v = 0;
    vs[1].pos = {24, 0};
    vs[1].u = umax;
    vs[1].v = 0;
    vs[2].pos = {0, 24};
    vs[2].u = umin;
    vs[2].v = 24;
    vs[3].pos = {24, 0};
    vs[3].u = umax;
    vs[3].v = 0;
    vs[4].pos = {0, 24};
    vs[4].u = umin;
    vs[4].v = 24;
    vs[5].pos = {24, 24};
    vs[5].u = umax;
    vs[5].v = 24;
    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
}

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_Player.pos = {windowSize.x / 2, windowSize.y / 2};

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
        vbufID_polyline = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Polyline");
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
        vs[0].u = 128;
        vs[0].v = 0;
        vs[1].u = texSize.x / 4 + 128;
        vs[1].v = 0;
        vs[2].u = 128;
        vs[2].v = texSize.y;
        vs[3].u = texSize.x / 4 + 128;
        vs[3].v = 0;
        vs[4].u = 128;
        vs[4].v = texSize.y;
        vs[5].u = texSize.x / 4 + 128;
        vs[5].v = texSize.y;

        vbufID_imageMilieu = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageMilieu");
    }

    // Préparation du drawcall du cercle qu'on peut bouger.
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");

    // Préparation du drawcall du prénom
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "Julien VERNAY", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Prenom");
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    bool bMoving = false;
    bool bPressedRun = false;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        float speed = CIRCLE_SPEED;
        if (gamepad.btn_right) {
            speed = CIRCLE_SPEED * 2;
            bPressedRun = true;
        }
        if (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
            bMoving = true;

        if (timeSinceStart >= g_Player.endHitAnim) {
            g_Player.pos.x += gamepad.stick_left_x * speed * deltaTime;
            g_Player.pos.y += gamepad.stick_left_y * speed * deltaTime;
        }

        if (gamepad.stick_left_x < 0)
            g_Player.bLeft = true;
        if (gamepad.stick_left_x > 0)
            g_Player.bLeft = false;

        if (gamepad.btn_left) {
            g_Player.endHitAnim = timeSinceStart + 3;
        }
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

    // Dessin du dinosaure.
    {
        uint64_t vbufID = DinoPlayer_GenerateVertexBuffer(g_Player, timeSinceStart, bMoving, bPressedRun);
        XDino_Draw(vbufID, texID_dino, g_Player.pos, 4);
        XDino_DestroyVertexBuffer(vbufID);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    // Affiche le prénom.
    {
        float tx = (renderSize.x - textSize_prenom.x * 2);
        float ty = (renderSize.y - textSize_prenom.y * 2);
        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {tx, ty}, 2);
    }

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur CTRL.
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.shoulder_left) {
        int diff = 0;
        if (keyboard.dpad_up)
            diff -= 1;
        if (keyboard.dpad_down)
            diff += 1;
        XDino_DrawStats(diff);
    }
#endif
}

void Dino_GameShut()
{
    XDino_DestroyVertexBuffer(vbufID_prenom);
    XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyGpuTexture(texID_imageMilieu);
    XDino_DestroyGpuTexture(texID_dino);
}