/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t vbufID_polyline;

uint64_t vbuffID_nom;
DinoVec2 text_Size_nom;


std::vector<anim> anims;
anim Get_Current_Anim();


anim Get_Current_Anim(DinoPlayer player)
{
    for (auto a : anims) {
        if (a.state == player.m_currentAnim)
            return a;
    }

    return anims[0];
}


std::vector<DinoPlayer> g_players = {};

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.


void Dino_GameInit()
{
    g_players.resize(4);
    g_players[0].Init(0);
    g_players[1].Init(1);
    g_players[2].Init(2);
    g_players[3].Init(3);

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

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

    // Set up Anims.
    {
        anims.push_back({Idle, 0, 4, 8});
        anims.push_back({Walk, 96, 6, 8});
        anims.push_back({Hit, 336, 3, 8});
        anims.push_back({Run, 432, 6, 16});
    }

    // Préparation du drawCall du nom
    {
        std::vector<DinoVertex> vs;
        text_Size_nom = Dino_GenVertices_Text(vs, "Betremieux Elouan", DinoColor_WHITE, DinoColor_GREY);
        vbuffID_nom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

}


void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.

    DinoGamepad gamepad{};
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad))
        g_players[0].Update(timeSinceStart, deltaTime, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad))
        g_players[1].Update(timeSinceStart, deltaTime, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad))
        g_players[2].Update(timeSinceStart, deltaTime, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad))
        g_players[3].Update(timeSinceStart, deltaTime, gamepad);

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

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

    // Affichage du nom
    {
        XDino_Draw(vbuffID_nom,
                   XDino_TEXID_FONT,
                   {renderSize.x - text_Size_nom.x * 2, renderSize.y - text_Size_nom.y * 2},
                   2);
    }

    // Affichage du player
    {
        for (DinoPlayer& player : g_players) {
            player.Draw(timeSinceStart, Get_Current_Anim(player));
        }
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
    for (DinoPlayer& player : g_players) {
        player.Shut();
    }
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyVertexBuffer(vbuffID_nom);
}