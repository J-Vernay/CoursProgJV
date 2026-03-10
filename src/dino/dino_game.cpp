/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;
DinoVec2 g_dinoPos = {};
bool g_bLeft = false;

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
uint64_t vbufID_dino;
uint64_t texID_imageMilieu;
uint64_t texID_dino;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.
constexpr uint16_t player_anim_nbFrames_idle = 4;
constexpr uint16_t player_anim_FPS_idle = 8;
constexpr uint16_t player_anim_base_idle = 0;

constexpr uint16_t player_anim_nbFrames_walk = 6;
constexpr uint16_t player_anim_FPS_walk = 8;
constexpr uint16_t player_anim_base_walk = 96;

constexpr uint16_t player_anim_nbFrames_hurt = 3;
constexpr uint16_t player_anim_FPS_Hurt = 8;
constexpr uint16_t player_anim_base_hurt = 336;

constexpr uint16_t player_anim_nbFrames_run = 6;
constexpr uint16_t player_anim_FPS_run = 16;
constexpr uint16_t player_anim_base_run = 432;

uint16_t player_anim_nbFrames_current = 4;
uint16_t player_anim_FPS_current = 8;
uint16_t player_anim_base_current = 0;
uint16_t indexFrame = 0;

double g_endHurtAnimation = 0;

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_dinoPos = {windowSize.x / 2, windowSize.y / 2};

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
        textSize_prenom = Dino_GenVertices_Text(vs, "Barnabé BERGER", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Prenom");
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;
    indexFrame = static_cast<int>(timeSinceStart * player_anim_FPS_current) % player_anim_nbFrames_current;

    // Gestion des entrées et mise à jour de la logique de jeu.

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (g_endHurtAnimation > 0) {
            player_anim_nbFrames_current = player_anim_nbFrames_hurt;
            player_anim_FPS_current = player_anim_FPS_Hurt;
            player_anim_base_current = player_anim_base_hurt;
            g_endHurtAnimation -= deltaTime;
        }

        else {
            float speed = CIRCLE_SPEED;
            if (gamepad.btn_right)
                speed = CIRCLE_SPEED * 2;

            g_dinoPos.x += gamepad.stick_left_x * speed * deltaTime;
            g_dinoPos.y += gamepad.stick_left_y * speed * deltaTime;

            if (gamepad.stick_left_x != 0.f || gamepad.stick_left_y != 0.f) {
                //detecte si le player bouge
                player_anim_nbFrames_current = gamepad.btn_right ? player_anim_nbFrames_run : player_anim_nbFrames_walk;
                player_anim_FPS_current = gamepad.btn_right ? player_anim_FPS_run : player_anim_FPS_walk;
                player_anim_base_current = gamepad.btn_right ? player_anim_base_run : player_anim_base_walk;
            }

            if (gamepad.stick_left_x < 0) {
                g_bLeft = true;
            }

            else if (gamepad.stick_left_x > 0) {
                g_bLeft = false;
            }

            else if (gamepad.stick_left_y != 0.f) {
                player_anim_nbFrames_current = gamepad.btn_right ? player_anim_nbFrames_run : player_anim_nbFrames_walk;
                player_anim_FPS_current = gamepad.btn_right ? player_anim_FPS_run : player_anim_FPS_walk;
                player_anim_base_current = gamepad.btn_right ? player_anim_base_run : player_anim_base_walk;
            }

            else {
                player_anim_nbFrames_current = player_anim_nbFrames_idle;
                player_anim_FPS_current = player_anim_FPS_idle;
                player_anim_base_current = player_anim_base_idle;
            }

            if (gamepad.btn_left) {
                g_endHurtAnimation = 3;
            }
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
        std::vector<DinoVertex> vs;
        uint16_t umin, umax;
        if (g_bLeft) {
            umin = 24 * indexFrame + player_anim_base_current + 24;
            umax = 24 * indexFrame + player_anim_base_current;
        }
        else {
            umin = 24 * indexFrame + player_anim_base_current;
            umax = 24 * indexFrame + player_anim_base_current + 24;
        }

        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[0].u = umin;
        vs[0].v = 0;
        vs[1].pos = {1, 0};
        vs[1].u = umax;
        vs[1].v = 0;
        vs[2].pos = {0, 1};
        vs[2].u = umin;
        vs[2].v = 24;
        vs[3].pos = {1, 0};
        vs[3].u = umax;
        vs[3].v = 0;
        vs[4].pos = {0, 1};
        vs[4].u = umin;
        vs[4].v = 24;
        vs[5].pos = {1, 1};
        vs[5].u = umax;
        vs[5].v = 24;
        vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Circle");
        XDino_Draw(vbufID_dino, texID_dino, g_dinoPos, 96);
        XDino_DestroyVertexBuffer(vbufID_dino);
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