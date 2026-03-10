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
double g_dinoCurrentSpeed;
bool g_dinoGoingLeft = false;
float g_dinoAnimElapsed = 0;
int g_currFrame = 0;

uint64_t vbufID_polyline;
//uint64_t vbufID_imageMilieu;
//uint64_t texID_imageMilieu;
//uint64_t vbufID_circle;
uint64_t vbufID_dino;
uint64_t texID_dino;

uint64_t vbufID_prenom;
DinoVec2 textSize_Prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.
constexpr float DINO_RUN_SPEED = 0.5f * DINO_SPEED; // Running Speed
constexpr float DINO_SCALE = 2.f; // Dino Scale
constexpr int DINO_ANIM_FRAMES_PER_SECOND = 8;

// - All Corners of Dino Sprite
constexpr DinoVec2 UPPER_LEFT = {0, 0};
constexpr DinoVec2 UPPER_RIGHT = {24, 0};
constexpr DinoVec2 LOWER_LEFT = {0, 24};
constexpr DinoVec2 LOWER_RIGHT = {24, 24};

// - All anims lenght
constexpr int ANIM_IDLE_LEN = 4;
constexpr int ANIM_WALK_LEN = 6;
constexpr int ANIM_HURT_LEN = 3;
constexpr int ANIM_RUN_LEN = 6;


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
        // constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        // constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        // constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        // constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};
        //
        // texID_imageMilieu = XDino_CreateGpuTexture("animals.png");
        // DinoVec2 texSize = XDino_GetGpuTextureSize(texID_imageMilieu);
        //
        // std::vector<DinoVertex> vs;
        // vs.resize(6);
        // vs[0].pos = {-2, -1};
        // vs[0].color = PURPLE;
        // vs[1].pos = {2, -1};
        // vs[1].color = CYAN;
        // vs[2].pos = {-2, 1};
        // vs[2].color = PINK;
        // vs[3].pos = {2, -1};
        // vs[3].color = CYAN;
        // vs[4].pos = {-2, 1};
        // vs[4].color = PINK;
        // vs[5].pos = {2, 1};
        // vs[5].color = ORANGE;
        // vs[0].u = 0;
        // vs[0].v = 0;
        // vs[1].u = texSize.x;
        // vs[1].v = 0;
        // vs[2].u = 0;
        // vs[2].v = texSize.y;
        // vs[3].u = texSize.x;
        // vs[3].v = 0;
        // vs[4].u = 0;
        // vs[4].v = texSize.y;
        // vs[5].u = texSize.x;
        // vs[5].v = texSize.y;
        //
        // vbufID_imageMilieu = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageMilieu");
    }

    // Préparation du drawcall du cercle qu'on peut bouger.
    // {
    //     std::vector<DinoVertex> vs;
    //     Dino_GenVertices_Circle(vs, 20);
    //     vbufID_circle = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Circle");
    // }

    // Préparation du drawcall du nom en bas à droite
    {
        std::vector<DinoVertex> vs;
        textSize_Prenom = Dino_GenVertices_Text(vs, "Cedric Charrier", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

    // Preparing the texture for the dino
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");
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

        // if (gamepad.btn_down && !gamepad.btn_up)
        //     g_scale /= 1.01;
        // if (gamepad.btn_up && !gamepad.btn_down)
        //     g_scale *= 1.01;
        // if (gamepad.btn_left && !gamepad.btn_right)
        //     g_rotation += 90.0 * deltaTime;
        // if (gamepad.btn_right && !gamepad.btn_left)
        //     g_rotation -= 90.0 * deltaTime;

        // Prevent Diagonal speed boost
        if (gamepad.stick_left_x != 0.0f && gamepad.stick_left_y != 0.0f)
            g_dinoCurrentSpeed = 0.75f * DINO_SPEED;
        else
            g_dinoCurrentSpeed = DINO_SPEED;

        g_dinoPos.x += gamepad.stick_left_x * (g_dinoCurrentSpeed + gamepad.btn_right * DINO_RUN_SPEED) * deltaTime;
        g_dinoPos.y += gamepad.stick_left_y * (g_dinoCurrentSpeed + gamepad.btn_right * DINO_RUN_SPEED) * deltaTime;

        // Only change facing orientation if player is moving
        if (gamepad.stick_left_x != 0.0f) {
            g_dinoGoingLeft = gamepad.stick_left_x <= 0.0f; // Changes facing orientation based on input
        }

    }

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    // XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);

    // Dessin de la texture centrale qu'on peut bouger.
    // DinoVec2 translation = {renderSize.x / 2, renderSize.y / 2};
    // double scale = g_scale * std::min(renderSize.x, renderSize.y) / 4;
    // XDino_Draw(vbufID_imageMilieu, texID_imageMilieu, translation, scale, g_rotation);

    // Dessin du cercle que l'on peut bouger.
    //XDino_Draw(vbufID_circle, XDino_TEXID_FONT, g_circlePos);

    DinoVec2 renderSize = XDino_GetRenderSize();
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    // Affichage du nom en bas à droite
    {
        float tx = (renderSize.x - textSize_Prenom.x * 2);
        float ty = (renderSize.y - textSize_Prenom.y * 2);

        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {.x = tx, .y = ty}, 2);
    }

    // Choosing Animation based on current player behaviour
    int currAnimLen;
    int animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    int firstFrameOfAnim;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {

        if (gamepadIdx != DinoGamepadIdx::Gamepad1)
            continue;

        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        // Add hurt anim in first, to always be chosen over the others
        if (gamepad.stick_left_x == 0.0f && gamepad.stick_left_y == 0.0f) {
            currAnimLen = ANIM_IDLE_LEN;
            firstFrameOfAnim = 0;
            animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
        }
        else if (gamepad.btn_right == 0.0f) {
            currAnimLen = ANIM_WALK_LEN;
            firstFrameOfAnim = ANIM_IDLE_LEN;
            animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
        }
        else {
            currAnimLen = ANIM_RUN_LEN;
            firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + ANIM_HURT_LEN + 4 + 1;
            animationSpeed = DINO_ANIM_FRAMES_PER_SECOND * 2;
        }

        if (g_dinoAnimElapsed > 1.f / animationSpeed) {
            g_currFrame = (g_currFrame + 1) % currAnimLen;
            g_dinoAnimElapsed = 0.0f;
        }
        else {
            g_dinoAnimElapsed += deltaTime;
        }
    }

    // Displaying Dino
    {
        std::vector<DinoVertex> vs;
        vs.resize(6);

        vs[0].pos = g_dinoGoingLeft ? UPPER_RIGHT : UPPER_LEFT;
        vs[0].u = 0 + (firstFrameOfAnim + g_currFrame) * 24;
        vs[0].v = 0;

        vs[1].pos = g_dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
        vs[1].u = 24 + (firstFrameOfAnim + g_currFrame) * 24;
        vs[1].v = 0;

        vs[2].pos = g_dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
        vs[2].u = 0 + (firstFrameOfAnim + g_currFrame) * 24;
        vs[2].v = 24;

        vs[3].pos = g_dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
        vs[3].u = 24 + (firstFrameOfAnim + g_currFrame) * 24;
        vs[3].v = 0;

        vs[4].pos = g_dinoGoingLeft ? LOWER_LEFT : LOWER_RIGHT;
        vs[4].u = 24 + (firstFrameOfAnim + g_currFrame) * 24;
        vs[4].v = 24;

        vs[5].pos = g_dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
        vs[5].u = 0 + (firstFrameOfAnim + g_currFrame) * 24;
        vs[5].v = 24;

        vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");

        XDino_Draw(vbufID_dino, texID_dino, g_dinoPos, DINO_SCALE);
        XDino_DestroyVertexBuffer(vbufID_dino);
    }

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur SHIFT.
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
    //XDino_DestroyVertexBuffer(vbufID_e);
    //XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    //XDino_DestroyVertexBuffer(vbufID_polyline);
    //XDino_DestroyGpuTexture(texID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_prenom);
}