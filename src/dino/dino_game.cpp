/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include "dino_animal.h"
#include "dino_lasso.h"
#include "dino_pause.h"
#include "dino_score.h"

#include <algorithm>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <map>
#include <dino/DinoController.h>
#include <dino/dino_terrain.h>
#include <dino/dino_tree.h>

#include <format>

constexpr double SPAWNTIME_BEGIN = 1;
constexpr double SPAWNTIME_END = 0.2;
constexpr double CHRONO_INIT = 60;

// Variables globales.
double g_lastTime = 0;

uint64_t vbufID_prenom;
DinoVec2 textSize_Prenom;

uint64_t texID_dino;

std::map<DinoGamepadIdx, DinoController> GamepadControllers;
std::vector<DinoController*> g_Players;
std::vector<DinoLasso> g_Lassos;
std::vector<DinoScore> g_Scores;
std::vector<DinoAnimal> g_Animals;
std::vector<DinoTree> g_Trees;

DinoTerrain g_terrain;

double g_timeSpawnAnimal = 0;
double g_chrono = CHRONO_INIT;

bool g_wasStartPressed = false;
bool g_lobby = true;
bool g_pause = false;

struct dpadState {
    bool prev_dpad_up = false;
    bool prev_dpad_down = false;
    bool prev_dpad_left = false;
    bool prev_dpad_right = false;
    bool prev_btn_right = false;
};

std::vector<dpadState> g_dpadStates;

constexpr DinoVec2 TERRAIN_SIZE = {24, 16};
constexpr DinoVec2 RENDER_SIZE = {480, 360};
constexpr int FLOWER_NUMBER_PER_TYPE = 10;
constexpr int SCORE_PER_ANIMAL = 10;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);
    
    DinoPause::Init(RENDER_SIZE);

    int playerCount = 0;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoController& controller = GamepadControllers[gamepadIdx];
        controller = {};

        g_Players.emplace_back(&controller);

        controller.Init(playerCount);
        playerCount++;
    }

    g_Lassos.resize(g_Players.size());
    g_Scores.resize(g_Players.size());
    g_dpadStates.resize(g_Players.size());
    if (g_Players.size() >= 1) {
        g_Lassos[0].Init(DinoColor_BLUE);
        g_Scores[0].Init(0, DinoColor_BLUE);
    }
    if (g_Players.size() >= 2) {
        g_Lassos[1].Init(DinoColor_RED);
        g_Scores[1].Init(1, DinoColor_RED);
    }
    if (g_Players.size() >= 3) {
        g_Lassos[2].Init(DinoColor_YELLOW);
        g_Scores[2].Init(2, DinoColor_YELLOW);
    }
    if (g_Players.size() >= 4) {
        g_Lassos[3].Init(DinoColor_GREEN);
        g_Scores[3].Init(3, DinoColor_GREEN);
    }

    // Préparation du drawcall du nom en bas à droite
    {
        std::vector<DinoVertex> vs;
        textSize_Prenom = Dino_GenVertices_Text(vs, "Cedric Charrier", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

    int idxSeason = XDino_RandomInt32(0, 3);
    g_terrain.Init(TERRAIN_SIZE, FLOWER_NUMBER_PER_TYPE, idxSeason);

    DinoVec2 terrainMin = g_terrain.GetTopLeft();
    DinoVec2 terrainMax = g_terrain.GetBottomRight();
    for (int i = 0; i < 4; ++i) {
        float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
        float y = terrainMin.y + 80;
        g_Trees.emplace_back(DinoVec2{x, y}, i, 0);
    }

    // Preparing the texture for the dino
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");

    // Preparing texture of animals
    DinoAnimal::InitTexture();
    DinoController::InitTexture();
    DinoTree::InitTexture();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique des dinos.

    // Lecture des inputs des joueurs
    bool pressedStart = false;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoController& controller = GamepadControllers[gamepadIdx];

        if (g_lobby) {
            if (gamepad.start) {
                controller.EnterGame();
                g_Lassos[controller.m_dinoNbr].isInGame = true;
                g_Scores[controller.m_dinoNbr].isInGame = true;
            }

            if (gamepad.select) {
                controller.QuitGame();
                g_Lassos[controller.m_dinoNbr].isInGame = false;
                g_Scores[controller.m_dinoNbr].isInGame = false;
            }
        }
        else {
            pressedStart = pressedStart || gamepad.start;
        }
        
        if (g_pause) {
            if (gamepad.dpad_up && !g_dpadStates[controller.m_dinoNbr].prev_dpad_up)
                DinoPause::Scroll(true);
            if (gamepad.dpad_down && !g_dpadStates[controller.m_dinoNbr].prev_dpad_down)
                DinoPause::Scroll(false);
            if (gamepad.dpad_left && !g_dpadStates[controller.m_dinoNbr].prev_dpad_left)
                DinoPause::ChangeTimer(true, g_chrono, CHRONO_INIT);
            if (gamepad.dpad_right && !g_dpadStates[controller.m_dinoNbr].prev_dpad_right)
                DinoPause::ChangeTimer(false, g_chrono, CHRONO_INIT);
            if (gamepad.btn_right && !g_dpadStates[controller.m_dinoNbr].prev_dpad_right) {
                int selection = DinoPause::GetCurrSelection();
                
                switch (selection) {
                    case 0:
                        g_chrono = CHRONO_INIT;
                        g_Animals.clear();
                        for (DinoScore& score : g_Scores) {
                            score.Reset();
                        }
                        g_pause = false;
                        break;
                    
                    case 1:
                        g_chrono = 0.01f;
                        g_pause = false;
                        break;
                    
                    case 2:
                        break;
                    
                    case 3:
                        g_pause = false;
                        break;
                }
            }
            
            // Update previous state
            g_dpadStates[controller.m_dinoNbr].prev_dpad_up = gamepad.dpad_up;
            g_dpadStates[controller.m_dinoNbr].prev_dpad_down = gamepad.dpad_down;
            g_dpadStates[controller.m_dinoNbr].prev_dpad_left = gamepad.dpad_left;
            g_dpadStates[controller.m_dinoNbr].prev_dpad_right = gamepad.dpad_right;
            g_dpadStates[controller.m_dinoNbr].prev_btn_right = gamepad.btn_right;
            
            continue;
        }

        controller.DinoMovement(gamepad, deltaTime);
    }

    if (!g_lobby) {
        if (pressedStart && !g_wasStartPressed)
            g_pause = true;
        g_wasStartPressed = pressedStart;
    }

    // Managing animal logic

    // Purger les animaux qui sont morts.
    // /!\ std::remove ne supprime pas /!\ il déplace à la fin du tableau
    // Il faut ensuite appeler .erase() pour enlever les éléments.
    auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
    for (auto it2 = it; it2 < g_Animals.end(); ++it2)
        it2->Shut();
    g_Animals.erase(it, g_Animals.end());

    if (timeSinceStart > g_timeSpawnAnimal && !g_lobby && !g_pause) {
        DinoAnimal& animal = g_Animals.emplace_back();
        EAnimalKind kind = (EAnimalKind)XDino_RandomInt32(0, 7);

        DinoVec2 min = g_terrain.GetTopLeft();
        DinoVec2 max = g_terrain.GetBottomRight();
        float x = XDino_RandomFloat(min.x, max.x);
        float y = XDino_RandomFloat(min.y, max.y);

        animal.Init(kind, {x, y}, timeSinceStart);
        double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * g_chrono;
        g_timeSpawnAnimal = timeSinceStart + spawnTime;
    }
    for (DinoAnimal& animal : g_Animals)
        if (!g_pause)
            animal.Update(deltaTime);

    // Pointeur de DinoEntity peut pointer vers DinoPlayer/DinoAnimal
    // car il y a un lien d'héritage.
    std::vector<DinoEntity*> entities;
    for (DinoController* player : g_Players)
        entities.emplace_back(player);

    for (DinoAnimal& animal : g_Animals)
        if (!g_lobby)
            entities.emplace_back(&animal);

    if (g_lobby) {
        for (DinoTree& tree : g_Trees) {
            entities.emplace_back(&tree);
            tree.Update(timeSinceStart, deltaTime);
        }
    }

    {
        for (size_t idxA = 0; idxA < entities.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
                DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

        for (DinoEntity* pEntity : entities)
            pEntity->ApplyTerrainLimit(g_terrain);

        if (g_Lassos.size() != g_Players.size())
            DINO_CRITICAL("Il devrait y avoir autant de lassos que de joueurs");
        for (int i = 0; i < g_Lassos.size(); ++i)
            g_Lassos[i].Update(g_Players[i]->GetPos());

        for (size_t idxA = 0; idxA < g_Lassos.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < g_Lassos.size(); ++idxB)
                DinoLasso::ResolveCollision(g_Lassos[idxA], g_Lassos[idxB]);

        std::vector<EAnimalKind> kinds;
        EAnimalKind currKind;
        int sameKindCount = 0;
        int score = 0;
        for (int i = 0; i < g_Lassos.size(); ++i) {
            DinoLasso& lasso = g_Lassos[i];
            for (DinoEntity* pEntity : entities) {
                if (lasso.WasInLoop(pEntity->GetPos())) {
                    if (pEntity->GetKind() != EAnimalKind::Other) {
                        currKind = pEntity->GetKind();
                        kinds.emplace_back(currKind);
                        for (EAnimalKind kind : kinds) {
                            if (kind == currKind)
                                sameKindCount++;
                        }
                    }
                    score = SCORE_PER_ANIMAL * sameKindCount;
                    
                    if (i != pEntity->GetPlayerNbr()) // prevents auto loop
                        pEntity->ReactLoop(timeSinceStart, score, lasso.m_color);
                        
                    g_Scores[i].AddScore(score);
                    sameKindCount = 0;
                }
            }
            kinds.clear();
            currKind = EAnimalKind::Other;
        }
    }

    if (g_lobby) {
        for (DinoTree& tree : g_Trees)
            if (tree.WasLooped()) {
                // Lobby -> Gameplay
                g_lobby = false;
                g_terrain.Shut();
                g_terrain.Init(TERRAIN_SIZE, FLOWER_NUMBER_PER_TYPE, tree.GetIdxSeason());
                g_Trees.clear();

                for (DinoScore& score : g_Scores) {
                    score.Reset();
                }

                break;
            }
    }

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    // Décrémenter le chronomètre.
    if (!g_lobby && !g_pause)
        g_chrono -= deltaTime;

    if (g_chrono <= 0.f && !g_lobby) {
        g_lobby = true;
        g_chrono = CHRONO_INIT;

        g_Animals.clear();

        DinoVec2 terrainMin = g_terrain.GetTopLeft();
        DinoVec2 terrainMax = g_terrain.GetBottomRight();
        for (int i = 0; i < 4; ++i) {
            float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
            float y = terrainMin.y + 80;
            g_Trees.emplace_back(DinoVec2{x, y}, i, 5);
        }
    }

    // ---------------
    // -- Affichage -- 
    // ---------------
    
    g_terrain.DrawBG();
    g_terrain.DrawTerrain();
    g_terrain.DrawFlwrs();

    for (DinoLasso& lasso : g_Lassos)
        lasso.Draw();

    for (DinoScore& score : g_Scores)
        score.DrawScore();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);
    
    for (DinoAnimal& animal : g_Animals) {
        animal.DrawScoreText(deltaTime);
    }

    // Affichage du menu Pause
    if (g_pause) {
        DinoPause::DrawPauseMenu();
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

    DinoVec2 renderSize = XDino_GetRenderSize();
    // Chrono
    {
        std::string text = std::format("{:.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (renderSize.x - textSize.x * 2) / 2;
        float ty = 0;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

    // Affichage du nom en bas à droite
    {
        float tx = (renderSize.x - textSize_Prenom.x * 2);
        float ty = (renderSize.y - textSize_Prenom.y * 2);

        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {.x = tx, .y = ty}, 2);
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
    XDino_DestroyVertexBuffer(vbufID_prenom);

    XDino_DestroyGpuTexture(texID_dino);
    DinoAnimal::ShutTexture();
    DinoController::ShutTexture();
    DinoTree::ShutTexture();

    g_terrain.Shut();
}