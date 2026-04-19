/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <dino/dino_tree.h>
#include "FloatingText.h"

#include <format>
#include <algorithm>
#include <optional>

#include <unordered_map>


constexpr double SPAWNTIME_BEGIN = 1;
constexpr double SPAWNTIME_END = 0.033;
constexpr double CHRONO_INIT = 10;

// Variables globales.
double g_lastTime = 0;

struct PlayerState {
    DinoGamepadIdx gamepadIdx;
    DinoGamepad gamepad;
    DinoPlayer dino;
    DinoColor color;
    DinoLasso lasso;
    std::unordered_map<int, int> catchCountPerAnimalKind;
    int totalScore = 0;

    PlayerState(DinoGamepadIdx idx, DinoGamepad pad, int playerIdx, DinoColor playerColor)
    : gamepadIdx(idx), gamepad(pad), dino(playerIdx), color(playerColor), lasso(playerColor)
    {}
};

std::vector<DinoGamepadIdx> g_UnassignedGamepads;
std::vector<PlayerState> g_Players;
std::vector<DinoTree> g_Trees;
std::vector<FloatingText> g_FloatingTexts;

DinoTerrain g_Terrain;
std::vector<DinoAnimal> g_Animals;
double g_timeSpawnAnimal = 0;
double g_chrono = CHRONO_INIT;
bool g_bWasStartPressed = false;
bool g_bPause = false;
bool g_bLobby = true;

std::optional<DinoVertexBuffer> g_vbufID_prenom;
DinoVec2 textSize_prenom;




//pause menu
int g_pauseMenuSelectedIdx = 0;
bool g_bWasDpadUpPressed = false;
bool g_bWasDpadDownPressed = false;
bool g_bWasDpadLeftPressed = false;
bool g_bWasDpadRightPressed = false;
bool g_bWasBtnRightPressed = false;

//=============================

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

constexpr DinoVec2 RENDER_SIZE = {480, 360};




bool isPaused;


void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);

    DinoPlayer::InitStatic();
    DinoAnimal::InitStatic();
    DinoTree::InitStatic();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        g_UnassignedGamepads.emplace_back(idx);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_Terrain.Init(RENDER_SIZE, idxSeason);

    // Préparation du drawcall du prénom
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "warrick VEDRENNE", DinoColor_WHITE, DinoColor_GREY);
        g_vbufID_prenom.emplace(vs.data(), vs.size(), "Prenom");
    }

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();
    for (int i = 0; i < 4; ++i) {
        float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
        float y = terrainMin.y + 80;
        g_Trees.emplace_back(DinoVec2{x, y}, i);
    }

}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    XDino_SetRenderSize(RENDER_SIZE);

    // Gestion des entrées et mise à jour de la logique de jeu.

    constexpr DinoColor PLAYER_COLORS[4] = {
        DinoColor_BLUE,
        DinoColor_RED,
        DinoColor_YELLOW,
        DinoColor_GREEN,
    };

    // Détection des nouveaux joueurs

    if (g_bLobby) {

        for (DinoTree& tree : g_Trees)
            tree.Update(timeSinceStart, deltaTime);

        
        // Connexion d'un nouveau joueur
        for (int i = 0; i < (int)g_UnassignedGamepads.size(); i++) {
            DinoGamepadIdx gamepadIdx = g_UnassignedGamepads[i];
            DinoGamepad gamepad;
            if (XDino_GetGamepad(gamepadIdx, gamepad) && gamepad.start) {
                int idxNewPlayer = (int)g_Players.size();
                if (idxNewPlayer < 4) {
                    g_Players.emplace_back(gamepadIdx, gamepad, idxNewPlayer, PLAYER_COLORS[idxNewPlayer]);
                    g_UnassignedGamepads.erase(g_UnassignedGamepads.begin() + i);
                }
                break;
            }
        }

        // Déconnexion d'un joueur 
        for (int i = 0; i < (int)g_Players.size(); i++) {
            PlayerState& playerState = g_Players[i];
            DinoGamepad gamepad;
            if (XDino_GetGamepad(playerState.gamepadIdx, gamepad) && gamepad.select) {
                g_UnassignedGamepads.emplace_back(playerState.gamepadIdx);
                playerState.dino.Shut();
                g_Players.erase(g_Players.begin() + i);
                i--;
            }
        }
    }

    // Lecture des inputs des joueurs
    bool bPressedStart = false;
    for (PlayerState& player : g_Players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad)) {
            player.gamepad = gamepad;
            bPressedStart = bPressedStart || gamepad.start ;

            if(gamepad.select) { bPressedStart = false;}
        }
    }


    
    // Mettre en pause le jeu
    if (!g_bLobby) {
        if (bPressedStart && !g_bWasStartPressed) {
            g_bPause = !g_bPause; // g_bPause prend l'inverse de g_bPause
             g_pauseMenuSelectedIdx = 0;
        }
        g_bWasStartPressed = bPressedStart;
    }

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();

    if (!g_bPause) {
        for (PlayerState& player : g_Players)
            player.dino.Update(timeSinceStart, deltaTime, player.gamepad);
    }
    if (!g_bPause && !g_bLobby) {
        // Purger les animaux qui sont morts.
        // /!\ std::remove ne supprime pas /!\ il déplace à la fin du tableau
        // Il faut ensuite appeler .erase() pour enlever les éléments.
        auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
        for (auto it2 = it; it2 < g_Animals.end(); ++it2)
            it2->Shut();
        g_Animals.erase(it, g_Animals.end());

        // Spawner un animal si besoin.
        if (timeSinceStart > g_timeSpawnAnimal) {
            EAnimalKind kind = (EAnimalKind)XDino_RandomInt32(0, 7);

            float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
            float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);

            DinoAnimal& animal = g_Animals.emplace_back(timeSinceStart, kind, DinoVec2{x, y});
            double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) *
                g_chrono;
            g_timeSpawnAnimal = timeSinceStart + spawnTime;
        }

        // Update les animaux.
        for (DinoAnimal& animal : g_Animals)
            animal.Update(timeSinceStart, deltaTime);
    }

    // Pointeur de DinoEntity peut pointer vers DinoPlayer/DinoAnimal
    // car il y a un lien d'héritage.
    std::vector<DinoEntity*> entities;
    for (PlayerState& player : g_Players)
        entities.emplace_back(&player.dino);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);
    if (g_bLobby) {
        for (DinoTree& tree : g_Trees)
            entities.emplace_back(&tree);
    }

    if (!g_bPause) {
        for (size_t idxA = 0; idxA < entities.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
                DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

        for (DinoEntity* pEntity : entities)
            pEntity->ApplyLimit(terrainMin, terrainMax);

        for (PlayerState& player : g_Players)
            player.lasso.Update(player.dino.GetPos());

        for (size_t idxA = 0; idxA < g_Players.size(); ++idxA)
            for (size_t idxB = idxA + 1; idxB < g_Players.size(); ++idxB)
                DinoLasso::ResolveCollision(g_Players[idxA].lasso, g_Players[idxB].lasso);


        //=====================================================================
        for (size_t idxPlayer = 0; idxPlayer < g_Players.size(); ++idxPlayer)
        {
            PlayerState& player = g_Players[idxPlayer];
            for (DinoEntity* pEntity : entities)
            {
                if (!player.lasso.WasInLoop(pEntity->GetPos()))
                    continue;

                pEntity->ReactLoop(timeSinceStart);

                DinoAnimal* pAnimal = dynamic_cast<DinoAnimal*>(pEntity);
                if (!pAnimal)
                    continue;

                int animalKindKey = static_cast<int>(pAnimal->GetKind());
                int& catchCount = player.catchCountPerAnimalKind[animalKindKey];
                catchCount++;
                int pointsEarned = catchCount * 10;
                player.totalScore += pointsEarned;

                std::string pointsText = std::format("+{}", pointsEarned);
                g_FloatingTexts.emplace_back(pAnimal->GetPos(), pointsText, player.color);
            }
        }

        //================================================================================


        auto expiredIt = std::remove_if(g_FloatingTexts.begin(), g_FloatingTexts.end(),
           [](const FloatingText& ft) { return ft.IsExpired(); });
        g_FloatingTexts.erase(expiredIt, g_FloatingTexts.end());

        for (FloatingText& ft : g_FloatingTexts)
            ft.Update(deltaTime);
    }

    if (g_bLobby) {
        for (DinoTree& tree : g_Trees)
            if (tree.WasLooped()) {
                // Lobby -> Gameplay
                g_bLobby = false;

				g_chrono = CHRONO_INIT;
    			g_timeSpawnAnimal = 0;


                g_Terrain.Shut();
                g_Terrain.Init(RENDER_SIZE, tree.GetIdxSeason());
                g_Trees.clear();
                break;
            }
    }

    if (!g_bPause && !g_bLobby) {
        // Décrémenter le chronomètre.
        g_chrono -= deltaTime;


        if (g_chrono <= 0) {
            g_bLobby = true;
            g_chrono = CHRONO_INIT;

            for (PlayerState& player : g_Players)
            {
                player.catchCountPerAnimalKind.clear();
                player.totalScore = 0;
            }
            g_FloatingTexts.clear();

            for (auto animal : g_Animals) {
                animal.Shut();
                
            }

            

            g_Animals.clear();

            DinoVec2 terrainMin = g_Terrain.GetTopLeft();
            DinoVec2 terrainMax = g_Terrain.GetBottomRight();
            for (int i = 0; i < 4; ++i) {
                float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
                float y = terrainMin.y + 80;
                DinoTree& newTree = g_Trees.emplace_back(DinoVec2{x, y}, i);
                newTree.timer = 5.f;
            }
        }
    }

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    g_Terrain.Draw();

    for (PlayerState& player : g_Players)
        player.lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);

    for (const FloatingText& ft : g_FloatingTexts)
        ft.Draw();



    // Affichage des scores joueurs
    {
        float scoreDisplayX = 2.f;
        float scoreDisplayY = 20.f;
        float currentY = scoreDisplayY;

        for (size_t i = 0; i < g_Players.size(); i++)
        {
            const PlayerState& player = g_Players[i];
            std::string scoreText = std::format("P{} : {}", i + 1, player.totalScore);

            std::vector<DinoVertex> scoreVertices;
            DinoVec2 scoreTextSize = Dino_GenVertices_Text(scoreVertices, scoreText, player.color, DinoColor_TRANSPARENT);
            DinoVertexBuffer scoreVertexBuffer(scoreVertices.data(), scoreVertices.size(), "Score");

            XDino_Draw(scoreVertexBuffer.Get(), XDino_TEXID_FONT, {scoreDisplayX, currentY}, 2);
            currentY += scoreTextSize.y * 2 + 2.f; 
        }
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);

        // vs : Stocke une allocation mémoire sur le CPU
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);

        // vbufID : Représente une allocation mémoire sur la carte graphique
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "dTime");

        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {}, 2);

        // Destructeur de 'vbuf' appelé implicitement par le compilateur
        // Destructeur de 'vs' appelé implicitement par le compilateur
    }

    if (g_bPause)
    {
        bool bDpadUp = false, bDpadDown = false;
        bool bDpadLeft = false, bDpadRight = false;
        bool bBtnRight = false;
        bool bConfirm = false;

        for (PlayerState& player : g_Players)
        {
            DinoGamepad gamepad;
            if (XDino_GetGamepad(player.gamepadIdx, gamepad))
            {
                bDpadUp    = bDpadUp    || gamepad.dpad_up;
                bDpadDown  = bDpadDown  || gamepad.dpad_down;
                bDpadLeft  = bDpadLeft  || gamepad.dpad_left;
                bDpadRight = bDpadRight || gamepad.dpad_right;
                bBtnRight  = bBtnRight  || gamepad.btn_right;
                bConfirm   = bConfirm   || gamepad.select;
            }
        }

        DinoGamepad keyboardGamepad;
        if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboardGamepad))
        {
            bDpadUp    = bDpadUp    || keyboardGamepad.dpad_up;
            bDpadDown  = bDpadDown  || keyboardGamepad.dpad_down;
            bDpadLeft  = bDpadLeft  || keyboardGamepad.dpad_left;
            bDpadRight = bDpadRight || keyboardGamepad.dpad_right;
            bConfirm   = bConfirm   || keyboardGamepad.select;
        }

        constexpr int PAUSE_MENU_ITEM_COUNT = 4;

        if (bDpadUp && !g_bWasDpadUpPressed)
            g_pauseMenuSelectedIdx = (g_pauseMenuSelectedIdx - 1 + PAUSE_MENU_ITEM_COUNT) % PAUSE_MENU_ITEM_COUNT;
        if (bDpadDown && !g_bWasDpadDownPressed)
            g_pauseMenuSelectedIdx = (g_pauseMenuSelectedIdx + 1) % PAUSE_MENU_ITEM_COUNT;

        if (g_pauseMenuSelectedIdx == 2)
        {
            if (bDpadLeft && !g_bWasDpadLeftPressed)
                g_chrono = std::max(0.0, g_chrono - 10.0);
            if (bDpadRight && !g_bWasDpadRightPressed)
                g_chrono += 10.0;
        }

        if (bConfirm && !g_bWasBtnRightPressed)
        {
            if (g_pauseMenuSelectedIdx == 0) // Recommencer
            {
                for (DinoAnimal& animal : g_Animals)
                    animal.Shut();
                g_Animals.clear();
                g_FloatingTexts.clear();
                for (PlayerState& player : g_Players)
                {
                    player.catchCountPerAnimalKind.clear();
                    player.totalScore = 0;
                }
                g_chrono = CHRONO_INIT;
                g_timeSpawnAnimal = 0;
                g_bPause = false;
            }
            else if (g_pauseMenuSelectedIdx == 1) // Retour au lobby
            {
                for (DinoAnimal& animal : g_Animals)
                    animal.Shut();
                g_Animals.clear();
                g_FloatingTexts.clear();
                for (PlayerState& player : g_Players)
                {
                    player.catchCountPerAnimalKind.clear();
                    player.totalScore = 0;
                }
                g_chrono = CHRONO_INIT;
                g_bPause = false;
                g_bLobby = true;

                DinoVec2 tMin = g_Terrain.GetTopLeft();
                DinoVec2 tMax = g_Terrain.GetBottomRight();
                for (int i = 0; i < 4; ++i)
                {
                    float x = tMin.x + (1 + i) * ((tMax.x - tMin.x) / 5);
                    float y = tMin.y + 80;
                    DinoTree& newTree = g_Trees.emplace_back(DinoVec2{x, y}, i);
                    newTree.timer = 5.f;
                }
            }
            else if (g_pauseMenuSelectedIdx == 3) // Reprendre
            {
                g_bPause = false;
            }
        }

        g_bWasDpadUpPressed    = bDpadUp;
        g_bWasDpadDownPressed  = bDpadDown;
        g_bWasDpadLeftPressed  = bDpadLeft;
        g_bWasDpadRightPressed = bDpadRight;
        g_bWasBtnRightPressed  = bConfirm;

        constexpr float MENU_SCALE = 2.f;
        constexpr float MENU_MARGIN = 4.f;

        const std::string menuItemLabels[PAUSE_MENU_ITEM_COUNT] = {
            "Recommencer",
            "Retour au lobby",
            std::format("Chrono  < {:.0f}s >", g_chrono),
            "Reprendre"
        };

        float currentMenuY = RENDER_SIZE.y / 2 - 30.f;

        for (int i = 0; i < PAUSE_MENU_ITEM_COUNT; i++)
        {
            DinoColor itemColor = (i == g_pauseMenuSelectedIdx) ? DinoColor_YELLOW : DinoColor_WHITE;
            std::vector<DinoVertex> menuItemVertices;
            DinoVec2 menuItemSize = Dino_GenVertices_Text(menuItemVertices, menuItemLabels[i], itemColor, DinoColor_TRANSPARENT);
            DinoVertexBuffer menuItemBuffer(menuItemVertices.data(), menuItemVertices.size(), "MenuItem");

            float tx = (RENDER_SIZE.x - menuItemSize.x * MENU_SCALE) / 2;
            XDino_Draw(menuItemBuffer.Get(), XDino_TEXID_FONT, {tx, currentMenuY}, MENU_SCALE);
            currentMenuY += menuItemSize.y * MENU_SCALE + MENU_MARGIN;
        }
    }


    
    {
        std::string text = std::format("{:.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (RENDER_SIZE.x - textSize.x * 2) / 2;
        float ty = 0;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

    // Affiche le prénom.
    {
        float tx = (RENDER_SIZE.x - textSize_prenom.x * 2);
        float ty = (RENDER_SIZE.y - textSize_prenom.y * 2);
        XDino_Draw(g_vbufID_prenom->Get(), XDino_TEXID_FONT, {tx, ty}, 2);
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

    for (PlayerState& player : g_Players)
        player.dino.Shut();
    for (DinoAnimal& animal : g_Animals)
        animal.Shut();
    g_Terrain.Shut();

    DinoTree::ShutStatic();
    DinoPlayer::ShutStatic();
    DinoAnimal::ShutStatic();

    g_vbufID_prenom.reset();
}