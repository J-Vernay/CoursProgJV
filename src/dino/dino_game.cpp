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

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
uint64_t texID_imageMilieu;

//dino variables
uint64_t texID_dino1;

bool isRunning = false;
bool isGoingLeft = false;

//dino animation variables
struct Anim {
    std::vector<int> posU;
    int framesPerSecond;
};

Anim* g_currentAnimationPos;
Anim g_idlePos;
Anim g_walkPos;
Anim g_hitPos;
Anim g_runPos;
//

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

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

        vbufID_imageMilieu = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageMilieu");
    }

    // Préparation du drawcall du dino qu'on peut bouger.
    {
        texID_dino1 = XDino_CreateGpuTexture("dinosaurs.png");
    }

    // Préparation du drawcall du prenom.
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "DEMAGNEE Aloys", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "name");
    }

    //Préparation des variables d'animation
    {
        g_currentAnimationPos = &g_idlePos;

        g_idlePos = {{0, 24, 48, 72}, 8};
        g_walkPos = {{96, 120, 144, 168, 192, 216}, 8};
        g_hitPos = {{336, 360, 384}, 8};
        g_runPos = {{432, 456, 480, 504, 528, 552}, 16};
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    bool isStatic = false;

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
        //turning background image
        /*if (gamepad.btn_left && !gamepad.btn_right)
            g_rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            g_rotation -= 90.0 * deltaTime;*/

        g_dinoPos.x += isRunning
                           ? gamepad.stick_left_x * DINO_SPEED * deltaTime * 3
                           : gamepad.stick_left_x * DINO_SPEED * deltaTime;
        g_dinoPos.y += isRunning
                           ? gamepad.stick_left_y * DINO_SPEED * deltaTime * 3
                           : gamepad.stick_left_y * DINO_SPEED * deltaTime;

        //checking if dino is going left or right
        if (isGoingLeft && gamepad.stick_left_x > 0)
            isGoingLeft = false;
        else if (!isGoingLeft && gamepad.stick_left_x < 0)
            isGoingLeft = true;

        //checking if dino is running
        isRunning = gamepad.btn_right;
        isStatic = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
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

    // Dessin du dino1 que l'on peut bouger.
    {
        if (isStatic)
            g_currentAnimationPos = &g_idlePos;
        else {
            g_currentAnimationPos = isRunning ? &g_runPos : &g_walkPos;
        }

        int index = isGoingLeft
                        ? (int)(timeSinceStart * g_currentAnimationPos->framesPerSecond) % (
                              g_currentAnimationPos->posU.size() - 1) + 1
                        : (int)(timeSinceStart * g_currentAnimationPos->framesPerSecond) % (
                              g_currentAnimationPos->posU.size() - 1);
        int u1 = (*g_currentAnimationPos).posU[index];
        int u2 = isGoingLeft
                     ? (*g_currentAnimationPos).posU[(index - 1) % g_currentAnimationPos->posU.size()]
                     : (*g_currentAnimationPos).posU[(index + 1) % g_currentAnimationPos->posU.size()];

        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {-1, -1};
        vs[0].color = DinoColor_WHITE;
        vs[1].pos = {1, -1};
        vs[1].color = DinoColor_WHITE;
        vs[2].pos = {-1, 1};
        vs[2].color = DinoColor_WHITE;
        vs[3].pos = {1, -1};
        vs[3].color = DinoColor_WHITE;
        vs[4].pos = {-1, 1};
        vs[4].color = DinoColor_WHITE;
        vs[5].pos = {1, 1};
        vs[5].color = DinoColor_WHITE;
        vs[0].u = u1;
        vs[0].v = 0;
        vs[1].u = u2;
        vs[1].v = 0;
        vs[2].u = u1;
        vs[2].v = 24;
        vs[3].u = u2;
        vs[3].v = 0;
        vs[4].u = u1;
        vs[4].v = 24;
        vs[5].u = u2;
        vs[5].v = 24;

        uint64_t vbufID_dino1 = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino1");
        XDino_Draw(vbufID_dino1, texID_dino1, g_dinoPos, 48);
        XDino_DestroyVertexBuffer(vbufID_dino1);
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
    {
        DinoVec2 translation = {renderSize.x - textSize_prenom.x * 2, renderSize.y - textSize_prenom.y * 2};
        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, translation, 2);
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

    XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyVertexBuffer(vbufID_prenom);
    XDino_DestroyGpuTexture(texID_imageMilieu);
    XDino_DestroyGpuTexture(texID_dino1);
}