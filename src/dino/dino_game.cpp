/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <unordered_map>

#include <format>


// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

std::vector<DinoPlayer> g_Players;
std::unordered_map<DinoGamepad, DinoPlayer*> g_gamepadPlayer;

uint64_t vbufID_polyline;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    DinoGamepad gamepad{};
    XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad);
    g_Players.reserve(4);
    g_Players.resize(1);
    g_Players[0].Init(0);
    g_gamepadPlayer.insert({gamepad, &g_Players[0]});

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

    XDino_SetRenderSize({480, 360});

    // Gestion des entrées et mise à jour de la logique de jeu.

    DinoGamepad gamepad{};
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad)) {
        g_Players[0].Update(timeSinceStart, deltaTime, gamepad);
    }

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad)) {
        if (g_gamepadPlayer.contains(gamepad)) {
            g_gamepadPlayer[gamepad]->Update(timeSinceStart, deltaTime, gamepad);
        }
        else {
            g_Players.emplace_back();
            DinoPlayer& player = g_Players.back();

            g_gamepadPlayer.emplace(gamepad, &player);
            player.Init(g_Players.size());
        }
    }

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad)) {
        if (g_gamepadPlayer.contains(gamepad)) {
            g_gamepadPlayer[gamepad]->Update(timeSinceStart, deltaTime, gamepad);
        }
        else {
            g_Players.emplace_back();
            DinoPlayer& player = g_Players.back();

            g_gamepadPlayer.emplace(gamepad, &player);
            player.Init(g_Players.size());
        }
    }

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad)) {
        if (g_gamepadPlayer.contains(gamepad)) {
            g_gamepadPlayer[gamepad]->Update(timeSinceStart, deltaTime, gamepad);
        }
        else {
            g_Players.emplace_back();
            DinoPlayer& player = g_Players.back();

            g_gamepadPlayer.emplace(gamepad, &player);
            player.Init(g_Players.size());
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

    // Dessin du dinosaure.
    for (DinoPlayer& player : g_Players)
        player.Draw(timeSinceStart);

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
    // For-range loop
    for (DinoPlayer& player : g_Players)
        player.Shut();

    XDino_DestroyVertexBuffer(vbufID_prenom);
    XDino_DestroyVertexBuffer(vbufID_polyline);
}