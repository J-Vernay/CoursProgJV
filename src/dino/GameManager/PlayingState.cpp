#include <dino/GameManager/PlayingState.h>

#include "LobbyState.h"

PlayingState::PlayingState(DinoGameState* dino_game_state, int season)
{
    m_dinoGameState = dino_game_state;
    m_dinoTerrain.emplace(season);
    g_timeLeft = PLAYING_TIME;
    g_spawnTimer = 0;
}

void PlayingState::EnterState()
{
}

void PlayingState::UpdateState(float deltaTime, double timeSinceStart)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;
        
        //pause button management
        if (gamepad.start && !lastFrameInputs_map.find(gamepadIdx)->second.start) {
            isPaused = !isPaused;
        }
        
        //updating dino_players
        if (m_dinoGameState->gamepadDino_map.contains(gamepadIdx) && !isPaused) {
            m_dinoGameState->gamepadDino_map.at(gamepadIdx).DinoPlayer_ReadGamePad(gamepad, deltaTime);
        }
        //
        
        lastFrameInputs_map[gamepadIdx] = gamepad;
    }
    
    //drawing terrain
    /*if (!m_dinoTerrain.has_value())
        m_dinoTerrain.emplace(XDino_RandomInt32(0, 3));*/
    m_dinoTerrain->DinoTerrain_Draw();    
    
    //logic active only in game
    //adding an animal, max amount is 50
    if (!isPaused) {
        if (g_spawnTimer > std::lerp(g_endSpawnDelay, g_baseSpawnDelay, g_timeLeft / PLAYING_TIME)
           && m_dinoGameState->animals.size() < 50) {
            int index = m_dinoGameState->animals.size();
            m_dinoGameState->animals.emplace_back(m_dinoGameState->g_terrainTopLeft, 8);
            m_dinoGameState->g_dinoEntityManager.AddEntity(&m_dinoGameState->animals[index]);
            m_dinoGameState->g_dinoEntityManager.AddEntity(&m_dinoGameState->animals[index]);
            g_spawnTimer = 0;
           }
        g_spawnTimer += deltaTime;
    }
    
    
    // Timer
    {
        if (!isPaused){
            g_timeLeft -= deltaTime;
            g_timeLeft = std::max(g_timeLeft, 0.0f);
        }

        std::string text = std::format("{:04.1f}", g_timeLeft);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, {240 - textSize.x / 2, 0}, 2);
    }
    
    //drawing all entities
    if (isPaused){
        m_dinoGameState->g_lassoManager.SimpleDrawLasso();
        m_dinoGameState->g_dinoEntityManager.DrawEntitiesWithTerrainClamping(PLAYING_TIME - g_timeLeft, m_dinoGameState->g_terrainTopLeft);
        //resolving all collisions between entities
        //m_dinoGameState->g_dinoEntityManager.DinoCollision_HandleCollisions(m_dinoGameState->g_terrainTopLeft);
    }
    else{
        m_dinoGameState->g_lassoManager.UpdateLassos(m_dinoGameState->g_dinoEntityManager.entities);
        m_dinoGameState->g_dinoEntityManager.UpdateAndDrawEntities(PLAYING_TIME - g_timeLeft, deltaTime);
        //resolving all collisions between entities
        m_dinoGameState->g_dinoEntityManager.DinoCollision_HandleCollisions(m_dinoGameState->g_terrainTopLeft);
    }
    
    if(isPaused)
    {
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, "Pause", DinoColor_WHITE, DinoColor_BLACK);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "PauseTxt");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, DinoVec2{240,180}-textSize*2.5, 5);
    }
    
    if (g_timeLeft <= 0) {
        m_dinoGameState->ChangeGameState(std::make_unique<LobbyState>(m_dinoGameState, XDino_RandomInt32(0,3)));
    }
}

void PlayingState::ExitState()
{
    for (dino_animal& animal : m_dinoGameState->animals) {
        m_dinoGameState->g_dinoEntityManager.RemoveEntity(animal);
    }
    m_dinoTerrain.reset();
}