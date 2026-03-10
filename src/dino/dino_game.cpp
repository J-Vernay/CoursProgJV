/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include "dino_player.h"
#include "dino/dino_terrain.h"

#include <format>
#include <iostream>
#include <ostream>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;
DinoVec2 g_dinoPos = {};
bool g_lookLeft = false;

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
constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

int imageIndex = 0;
double currentFrame = 0;
double animationSpeed = 4;

int startFrame = 0;
int numberOfFrames = 2;


AnimRange animations[COUNT] = {
    {0, 4, 4},
    {5, 6, 8},
    {17, 7, 10},
    {14, 3, 4}
};

int DinoState = 0;

void ChangeAnimation(int _state)
{
    AnimRange newAnim = animations[_state];
    startFrame = newAnim.start;
    currentFrame = startFrame;
    numberOfFrames = newAnim.count;
    animationSpeed = newAnim.speed;
}

void ChangeState(int newState)
{
    if (DinoState != newState) {
        DinoState = newState;
        ChangeAnimation(DinoState);
    }
}


void DrawDebugImage()
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

void DrawBackgroundLine(DinoVec2 windowSize)
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

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_dinoPos = {windowSize.x / 2, windowSize.y / 2};

    enum DinoState dinoState = IDLE;
    ChangeState(dinoState);

    

    // Préparation du drawcall de la polyline (zigzag en fond).
    //DrawBackgroundLine(windowSize);

    // Préparation du drawcall de l'image au milieu qu'on peut tourner.
    //DrawDebugImage();

    // Préparation du drawcall du cercle qu'on peut bouger.
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");

    // Préparation du drawcall du prénom
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "MARESCAUX Corentin", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Prenom");
    }
}

void UpdateAnimationFrame(float deltaTime, int& imageIndex)
{
    currentFrame += deltaTime * animationSpeed;
    if (currentFrame > startFrame + numberOfFrames)
        currentFrame = startFrame;

    imageIndex = currentFrame;
}

void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // --- LOGIQUE DES ENTRÉES ---
    float moveX = 0, moveY = 0;
    bool isRunning = false;

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL) {
        DinoGamepad gp;
        if (!XDino_GetGamepad(idx, gp))
            continue;
        moveX += gp.stick_left_x;
        moveY += gp.stick_left_y;
        if (gp.select)
            isRunning = true;
    }

    // --- MISE À JOUR DU JOUEUR ---
    float speed = isRunning ? DINO_SPEED * 2 : DINO_SPEED;
    float magnitude = std::sqrt(moveX * moveX + moveY * moveY);

    if (magnitude > 0.1f) {
        g_player.SetState(isRunning ? RUN : WALK);
        g_player.lookLeft = (moveX < 0);
    }
    else {
        g_player.SetState(IDLE);
    }

    g_player.pos.x += moveX * speed * deltaTime;
    g_player.pos.y += moveY * speed * deltaTime;
    g_player.Update(deltaTime);

    // --- AFFICHAGE ---
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE); // Fond
    g_player.Draw(texID_dino); // Joueur

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
    double scale = g_scale * std::min(renderSize.x, renderSize.y) / 2;
    XDino_Draw(vbufID_imageMilieu, texID_imageMilieu, translation, scale, g_rotation);

    // Dessin du dinosaure.
    {
        std::vector<DinoVertex> vs;
        uint16_t umin, umax;

        int imageIndex;
        UpdateAnimationFrame(deltaTime, imageIndex);

        if (g_lookLeft) {
            umin = 24 + (24 * imageIndex);
            umax = 0 + (24 * imageIndex);
        }
        else {
            umin = 0 + (24 * imageIndex);
            umax = 24 + (24 * imageIndex);
        }

        int characterNumber = 0;

        uint16_t voffset = 24 * characterNumber;

        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[0].u = umin;
        vs[0].v = voffset;
        vs[1].pos = {24, 0};
        vs[1].u = umax;
        vs[1].v = voffset;
        vs[2].pos = {0, 24};
        vs[2].u = umin;
        vs[2].v = 24 + voffset;
        vs[3].pos = {24, 0};
        vs[3].u = umax;
        vs[3].v = voffset;
        vs[4].pos = {0, 24};
        vs[4].u = umin;
        vs[4].v = 24 + voffset;
        vs[5].pos = {24, 24};
        vs[5].u = umax;
        vs[5].v = 24 + voffset;
        vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Circle");
        XDino_Draw(vbufID_dino, texID_dino, g_dinoPos, 4);
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
        float tscale = 2;
        float tx = (renderSize.x - textSize_prenom.x * tscale);
        float ty = (renderSize.y - textSize_prenom.y * tscale);
        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {tx, ty}, tscale);
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