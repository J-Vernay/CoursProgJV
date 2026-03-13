/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "DinoGameState.h"
#include "LassoManager.h"
#include "dino_animal.h"
#include "dino_player.h"
#include "dino_terrain.h"
#include "dino_EntityManager.h"

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>
#include <unordered_map>

// Variables globales.
double g_lastTime = 0;

uint64_t texID_dino;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;

float g_spawnTimer;
const float g_baseSpawnDelay = 3;
const float g_endSpawnDelay = 0.5f;

DinoVec2 g_terrainTopLeft;

const float PLAYING_TIME = 60;
float g_timeLeft;

#include <deque>

std::unordered_map<DinoGamepadIdx, dino_player> gamepadDino_map;
std::deque<dino_animal> animals;

DinoGameState g_gameState;
dino_EntityManager g_dinoEntityManager;
LassoManager g_lassoManager;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;


void Dino_GameInit()
{
    g_gameState.ChangeGameState(DinoGameState::Season1);

    //DinoVec2 windowSize = XDino_GetWindowSize();
    DinoVec2 windowSize = {480, 360};
    XDino_SetRenderSize(windowSize);

    // Préparation des textures.
    {
        texID_dino = XDino_CreateGpuTexture("dinosaurs.png");
    }

    g_terrainTopLeft = g_gameState.GetTopLeft();

    // Préparation du drawcall du prenom.
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "DEMAGNEE Aloys", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "name");
    }

    dino_animal::DinoAnimal_InitStatic();
    g_timeLeft = PLAYING_TIME;
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
    g_gameState.Update();

    //adding an animal, max amount is 20
    if (g_spawnTimer > std::lerp(g_endSpawnDelay, g_baseSpawnDelay, g_timeLeft / PLAYING_TIME) && animals.size() < 50) {
        int index = animals.size();
        animals.emplace_back(g_terrainTopLeft, 8);
        g_dinoEntityManager.AddEntity(&animals[index]);
        g_spawnTimer = 0;
    }
    g_spawnTimer += deltaTime;

    // Gestion des entrées et mise à jour de la logique de jeu.
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.btn_up) {

            g_gameState.ChangeGameState((DinoGameState::game_state)XDino_RandomInt32(2, 5));
        }

        //spawning dino_players
        if (!gamepadDino_map.contains(gamepadIdx)
            && (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
            //max player amount == 4
            && gamepadDino_map.size() < 4) {

            DinoVec2 windowSize = XDino_GetWindowSize();
            dino_player newPlayer = dino_player(
                {g_terrainTopLeft.x + 128, g_terrainTopLeft.y + 96},
                8,
                texID_dino,
                gamepadDino_map.size(),
                g_terrainTopLeft);

            auto [it,bInserted] = gamepadDino_map.emplace(gamepadIdx, newPlayer);
            g_dinoEntityManager.AddEntity(&it->second);
            g_lassoManager.AddLasso(it->second);
        }

        //updating dino_players
        if (gamepadDino_map.contains(gamepadIdx)) {
            gamepadDino_map.at(gamepadIdx).DinoPlayer_ReadGamePad(gamepad, deltaTime);
        }
        //
    }

    // Timer
    {
        g_timeLeft -= deltaTime;
        g_timeLeft = std::max(g_timeLeft, 0.0f);

        std::string text = std::format("{:04.1f}", g_timeLeft);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, {240 - textSize.x / 2, 0}, 2);
    }
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, {}, 2);
    }
    {
        DinoVec2 translation = {renderSize.x - textSize_prenom.x * 2, renderSize.y - textSize_prenom.y * 2};
        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, translation, 2);
    }

    //drawing all entities
    g_lassoManager.UpdateLassos(g_dinoEntityManager.entities);
    g_dinoEntityManager.UpdateAndDrawEntities(timeSinceStart, deltaTime);

    //resolving all collisions between entities
    g_dinoEntityManager.DinoCollision_HandleCollisions(g_terrainTopLeft);

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
    g_dinoTerrain.reset();
    dino_animal::DinoAnimal_ShutStatic();

    XDino_DestroyVertexBuffer(vbufID_prenom);

    XDino_DestroyGpuTexture(texID_dino);
}