/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.


#include <algorithm>
#include <dino/dino_player.h>

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <format>

// Variables globales.
double lastTime = 0;



std::vector<DinoPlayer> g_Players;



void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_Players.resize(4);
    g_Players[0].Init({windowSize.x / 2 - 100, windowSize.y / 2 - 100}, 0, DinoGamepadIdx::Keyboard);
    g_Players[1].Init({windowSize.x / 2 - 100, windowSize.y / 2 + 100}, 1, DinoGamepadIdx::Gamepad1);
    g_Players[2].Init({windowSize.x / 2 + 100, windowSize.y / 2 - 100}, 2, DinoGamepadIdx::Gamepad2);
    g_Players[3].Init({windowSize.x / 2 + 100, windowSize.y / 2 + 100}, 3, DinoGamepadIdx::Gamepad3);

    // Display the terrain using the terrains.png texture
    DinoDrawCall drawCall;
    drawCall.textureName = "terrains.png";
    drawCall.vertices.resize(6);
    drawCall.vertices[0].pos = {0, 0};
    drawCall.vertices[1].pos = {windowSize.x, 0};
    drawCall.vertices[2].pos = {0, windowSize.y};
    drawCall.vertices[3].pos = {windowSize.x, 0};
    drawCall.vertices[4].pos = {0, windowSize.y};
    drawCall.vertices[5].pos = {windowSize.x, windowSize.y};
    drawCall.vertices[0].u = 0;
    drawCall.vertices[0].v = 0;
    drawCall.vertices[1].u = 1;
    drawCall.vertices[1].v = 0;
    drawCall.vertices[2].u = 0;
    drawCall.vertices[2].v = 1;
    drawCall.vertices[3].u = 1;
    drawCall.vertices[3].v = 0;
    drawCall.vertices[4].u = 0;
    drawCall.vertices[4].v = 1;
    drawCall.vertices[5].u = 1;
    drawCall.vertices[5].v = 1;
    XDino_Draw(drawCall);
    
    
}


void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    for (DinoPlayer& player : g_Players)
    {
        player.UpdatePlayer(deltaTime);
    }

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    std::sort(g_Players.begin(), g_Players.end(), [](DinoPlayer const& a, DinoPlayer const& b) {
        return a.pos.y < b.pos.y;
    });

    /*// Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
{
    std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
    drawCall.scale = 2;
    XDino_Draw(drawCall);
}

//Prénom
{
    std::string text = "Thomas Druesne";
    DinoVec2 textSize;

    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &textSize);
    drawCall.scale = 2;
    // Placer en bas a droite dans le coin en utilisant le parametre de retour poutsize

    drawCall.translation.x = windowSize.x - 2 * textSize.x;
    drawCall.translation.y = windowSize.y - 2 * textSize.y;

    // Afficher le texte
    XDino_Draw(drawCall);
}*/

}


void Dino_GameShut()
{

}


/*// Dessin de la texture centrale qu'on peut bouger.
    {
        constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

        float quarterWidth = windowSize.x / 4;
        float quarterHeight = windowSize.y / 4;

        DinoDrawCall drawCall;
        drawCall.vertices.resize(6);
        drawCall.vertices[0].pos = {-quarterWidth, -quarterHeight};
        drawCall.vertices[0].color = PURPLE;
        drawCall.vertices[1].pos = {quarterWidth, -quarterHeight};
        drawCall.vertices[1].color = CYAN;
        drawCall.vertices[2].pos = {-quarterWidth, quarterHeight};
        drawCall.vertices[2].color = PINK;
        drawCall.vertices[3].pos = {quarterWidth, -quarterHeight};
        drawCall.vertices[3].color = CYAN;
        drawCall.vertices[4].pos = {-quarterWidth, quarterHeight};
        drawCall.vertices[4].color = PINK;
        drawCall.vertices[5].pos = {quarterWidth, quarterHeight};
        drawCall.vertices[5].color = ORANGE;

        drawCall.translation = {windowSize.x / 2, windowSize.y / 2};
        drawCall.rotation = rotation;
        drawCall.scale = scale;

        drawCall.vertices[0].u = 0;
        drawCall.vertices[0].v = 0;
        drawCall.vertices[1].u = 96;
        drawCall.vertices[1].v = 0;
        drawCall.vertices[2].u = 0;
        drawCall.vertices[2].v = 96;
        drawCall.vertices[3].u = 96;
        drawCall.vertices[3].v = 0;
        drawCall.vertices[4].u = 0;
        drawCall.vertices[4].v = 96;
        drawCall.vertices[5].u = 96;
        drawCall.vertices[5].v = 96;
        drawCall.textureName = "monogram-bitmap.png";

        XDino_Draw(drawCall);
    }*/