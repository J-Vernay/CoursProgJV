/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "DinoPlayer.h"

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;
double rotation = 360.0;
double scale = 1.0;
DinoVec2 windowSize;

// Constantes.
constexpr float speed = 300.f;
constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

// Forwards Declarations
void InitDinoPlayers(DinoVec2 windowSize);
DinoVec2 GetSpawnPos(int index, DinoVec2 windowSize);

// Variables
std::vector<DinoPlayer> players = {};
DinoPlayer dinoPlayer = DinoPlayer();

void Dino_GameInit()
{
    windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize({480, 360});
    InitDinoPlayers(windowSize);

    DinoDrawCall drawCall;
    drawCall.textureName = "terrain.png";
    drawCall.vertices.reserve(6);

    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {16, 0};
    DinoVec2 posC = {0, 16};
    DinoVec2 posD = {16, 16};
}

void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    for (DinoPlayer& player : players) {
        player.Update(deltaTime);
        player.Draw(timeSinceStart);
    }

    // DISPLAY

    // Default Color
    XDino_SetClearColor(CLEAR_COLOR);

    // Affichage du nom et prénom en bas a droite de l'écran.
    {
        std::string text = std::format("Rayan EL KOTOB");
        DinoVec2* textSize = new DinoVec2();
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, textSize);
        drawCall.translation = {windowSize.x - textSize->x * 2, windowSize.y - textSize->y * 2};
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        drawCall.scale = 2;
        XDino_Draw(drawCall);
    }

}

void InitDinoPlayers(DinoVec2 windowSize)
{
    players = std::vector<DinoPlayer>(4);
    for (int i = 0; i < players.size(); i++) {
        DinoPlayer player = DinoPlayer();
        player.Initialize(static_cast<DinoGamepadIdx>(i), GetSpawnPos(i, windowSize));
        players[i] = player;
    }
}

DinoVec2 GetSpawnPos(int index, DinoVec2 windowSize)
{
    return {windowSize.x * index / 4, windowSize.y / 2};
}

void DrawTerrain()
{
    
}

void Dino_GameShut(DinoVec2 windowSize)
{

}

// // Dessin de la texture centrale qu'on peut bouger.
// {
//     constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
//     constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
//     constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
//     constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};
//
//     float quarterWidth = windowSize.x / 4;
//     float quarterHeight = windowSize.y / 4;
//
//     DinoDrawCall drawCall;
//     drawCall.vertices.resize(6);
//     drawCall.vertices[0].pos = {-quarterWidth, -quarterHeight};
//     drawCall.vertices[0].color = PURPLE;
//     drawCall.vertices[1].pos = {quarterWidth, -quarterHeight};
//     drawCall.vertices[1].color = CYAN;
//     drawCall.vertices[2].pos = {-quarterWidth, quarterHeight};
//     drawCall.vertices[2].color = PINK;
//     drawCall.vertices[3].pos = {quarterWidth, -quarterHeight};
//     drawCall.vertices[3].color = CYAN;
//     drawCall.vertices[4].pos = {-quarterWidth, quarterHeight};
//     drawCall.vertices[4].color = PINK;
//     drawCall.vertices[5].pos = {quarterWidth, quarterHeight};
//     drawCall.vertices[5].color = ORANGE;
//
//     drawCall.translation = {windowSize.x / 2, windowSize.y / 2};
//     drawCall.rotation = rotation;
//     drawCall.scale = scale;
//
//     drawCall.vertices[0].u = 0;
//     drawCall.vertices[0].v = 0;
//     drawCall.vertices[1].u = 96;
//     drawCall.vertices[1].v = 0;
//     drawCall.vertices[2].u = 0;
//     drawCall.vertices[2].v = 96;
//     drawCall.vertices[3].u = 96;
//     drawCall.vertices[3].v = 0;
//     drawCall.vertices[4].u = 0;
//     drawCall.vertices[4].v = 96;
//     drawCall.vertices[5].u = 96;
//     drawCall.vertices[5].v = 96;
//     drawCall.textureName = "monogram-bitmap.png";
//
//     XDino_Draw(drawCall);
// }