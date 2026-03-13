/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "DinoGameState.h"
#include "LassoManager.h"
#include "dino_animal.h"
#include "dino_player.h"
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
std::unordered_map<DinoGamepadIdx, DinoGamepad> lastFrameInputs_map;
std::deque<dino_animal> animals;

DinoGameState g_gameState;
dino_EntityManager g_dinoEntityManager;
LassoManager g_lassoManager;


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
    }

    g_terrainTopLeft = {(480 - 256) / 2, (360 - 192) / 2};

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

    // Gestion des entrées et mise à jour de la logique de jeu.
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.start && !lastFrameInputs_map.find(gamepadIdx)->second.start) {
            if (g_gameState.GetState() == DinoGameState::Pause)
                g_gameState.ChangeGameState(DinoGameState::Playing);
            else
                g_gameState.ChangeGameState(DinoGameState::Pause);
        }

        if (gamepad.btn_up) {

            g_gameState.ChangeGameState(DinoGameState::Playing);
        }

        //spawning dino_players
        if (g_gameState.GetState() == DinoGameState::Lobby
            && !gamepadDino_map.contains(gamepadIdx)
            && (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
            //max player amount == 4
            && gamepadDino_map.size() < 4) {

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
        //despawning player
        if (g_gameState.GetState() == DinoGameState::Lobby
            && gamepadDino_map.contains(gamepadIdx)
            && gamepad.select) {

            dino_player& player = gamepadDino_map.find(gamepadIdx)->second;

            for (int i = g_dinoEntityManager.entities.size() - 1; i >= 0; i--) {
                if (g_dinoEntityManager.entities[i] == &player) {
                    g_dinoEntityManager.entities.erase(g_dinoEntityManager.entities.begin() + i);
                }
            }
            g_lassoManager.RemovePlayer(player);

            gamepadDino_map.find(gamepadIdx)->second.DinoPlayer_Despawn();
            gamepadDino_map.erase(gamepadIdx);
        }

        //updating dino_players
        if (gamepadDino_map.contains(gamepadIdx) && g_gameState.GetState() != DinoGameState::Pause) {
            gamepadDino_map.at(gamepadIdx).DinoPlayer_ReadGamePad(gamepad, deltaTime);
        }
        //
        lastFrameInputs_map[gamepadIdx] = gamepad;
    }

    //logic active only in game
    //adding an animal, max amount is 20
    if (g_gameState.GetState() == DinoGameState::Playing) {
        if (g_spawnTimer > std::lerp(g_endSpawnDelay, g_baseSpawnDelay, g_timeLeft / PLAYING_TIME) && animals.size() <
            50) {
            int index = animals.size();
            animals.emplace_back(g_terrainTopLeft, 8);
            g_dinoEntityManager.AddEntity(&animals[index]);
            g_dinoEntityManager.AddEntity(&animals[index]);
            g_spawnTimer = 0;
        }
        g_spawnTimer += deltaTime;
    }

    // Timer
    {
        if (g_gameState.GetState() == DinoGameState::Playing) {
            g_timeLeft -= deltaTime;
            g_timeLeft = std::max(g_timeLeft, 0.0f);
        }

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
    if (g_gameState.GetState() != DinoGameState::Pause) {
        g_lassoManager.UpdateLassos(g_dinoEntityManager.entities);
        g_dinoEntityManager.UpdateAndDrawEntities(PLAYING_TIME - g_timeLeft, deltaTime);
        //resolving all collisions between entities
        g_dinoEntityManager.DinoCollision_HandleCollisions(g_terrainTopLeft);
    }
    else {
        g_lassoManager.SimpleDrawLasso();
        g_dinoEntityManager.SimpleDrawEntities(PLAYING_TIME - g_timeLeft);

        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, "Paused", DinoColor_WHITE, DinoColor_BLACK);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, {240 - textSize.x * 2.5f, 180 - textSize.y * 2.5f}, 5);
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
    g_gameState.Shutdown();
    dino_animal::DinoAnimal_ShutStatic();

    XDino_DestroyVertexBuffer(vbufID_prenom);

    XDino_DestroyGpuTexture(texID_dino);
}