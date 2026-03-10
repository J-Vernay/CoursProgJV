/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <map>
#include <dino/DinoController.h>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t texID_dino;

std::map<DinoGamepadIdx, DinoControllerFields> GamepadControllers;

//uint64_t vbufID_imageMilieu;
//uint64_t texID_imageMilieu;
//uint64_t vbufID_circle;
uint64_t vbufID_polyline;

uint64_t vbufID_prenom;
DinoVec2 textSize_Prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    int playerCount = 0;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& fields = GamepadControllers[gamepadIdx];
        fields = {};
        fields.dinoPos = {windowSize.x / 2, windowSize.y / 2};
        fields.dinoColor = playerCount;
        playerCount++;
    }

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

        DinoControllerFields& fields = GamepadControllers[gamepadIdx];

        if (!fields.g_dinoCanTakeDamage)
            continue;

        fields.DinoMovement(gamepad, deltaTime);
    }

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    // XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);

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

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {

        // if (gamepadIdx != DinoGamepadIdx::Gamepad1)
        //     continue;

        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& fields = GamepadControllers[gamepadIdx];

        fields.vbufID_dino = fields.DrawDino(gamepad, deltaTime, texID_dino);
        XDino_DestroyVertexBuffer(fields.vbufID_dino);
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
    //XDino_DestroyGpuTexture(texID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyVertexBuffer(vbufID_prenom);
}