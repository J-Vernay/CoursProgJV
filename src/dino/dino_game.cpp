/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_animal.h"
#include "dino_player.h"
#include "dino_terrain.h"

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>
#include <unordered_map>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t texID_dino;
uint64_t texID_terrain;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;

float g_spawnTimer;
DinoVec2 g_terrainTopLeft;

std::unordered_map<DinoGamepadIdx, dino_player> gamepadDino_map;
std::vector<dino_animal> animals;
dino_terrain dinoTerrain;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;


void Dino_GameInit()
{
    //DinoVec2 windowSize = XDino_GetWindowSize();
    DinoVec2 windowSize = {480, 360};
    XDino_SetRenderSize(windowSize);

    // Préparation des textures.
    {
        texID_dino = XDino_CreateGpuTexture("dinosaurs.png");
        texID_terrain = XDino_CreateGpuTexture("terrain.png");
    }

    g_terrainTopLeft = dinoTerrain.DinoTerrain_Init(texID_terrain, XDino_RandomInt32(0, 3));

    // Préparation du drawcall du prenom.
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "DEMAGNEE Aloys", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "name");
    }

    dino_animal::DinoAnimal_InitStatic();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    DinoVec2 renderSize = XDino_GetRenderSize();

    //drawing terrain
    dinoTerrain.DinoTerrain_Draw();

    //adding an animal, max amount is 20
    if (g_spawnTimer > 1 && animals.size() < 20) {
        int index = animals.size();
        animals.push_back(dino_animal());
        animals[index].DinoAnimal_Spawn(g_terrainTopLeft);
        g_spawnTimer = 0;
    }
    g_spawnTimer += deltaTime;

    //drawing animals
    for (int i = animals.size() - 1; i >= 0; i--) {
        animals[i].DinoAnimal_Update(timeSinceStart, deltaTime);
    }

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

        //spawning dino_players
        if (!gamepadDino_map.contains(gamepadIdx)
            && (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
            //max player amount == 4
            && gamepadDino_map.size() < 4) {

            DinoVec2 windowSize = XDino_GetWindowSize();
            gamepadDino_map.emplace(
                gamepadIdx,
                dino_player(
                    {g_terrainTopLeft.x + 128, g_terrainTopLeft.y + 96},
                    texID_dino,
                    gamepadDino_map.size(),
                    g_terrainTopLeft));
        }

        //drawing dino_players
        if (gamepadDino_map.contains(gamepadIdx)) {
            gamepadDino_map.at(gamepadIdx).DinoCharacter_ReadGamepad(gamepad, deltaTime);
            gamepadDino_map.at(gamepadIdx).DinoCharacter_Update(timeSinceStart, deltaTime);
        }
        //
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
    dinoTerrain.DinoTerrain_ShutDown();
    dino_animal::DinoAnimal_ShutStatic();

    XDino_DestroyVertexBuffer(vbufID_prenom);

    XDino_DestroyGpuTexture(texID_dino);
    XDino_DestroyGpuTexture(texID_terrain);
}