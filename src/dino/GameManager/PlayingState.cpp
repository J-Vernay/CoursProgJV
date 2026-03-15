#include <dino/GameManager/PlayingState.h>

PlayingState::PlayingState(DinoGameState* dino_game_state, int Season)
{
    m_dinoGameState = dino_game_state;
}

void PlayingState::EnterState()
{
}

void PlayingState::UpdateState(float deltaTime, double timeSinceStart)
{
    
    //pause button management
    /*if (g_gameState.GetState() != DinoGameState::Lobby
            && gamepad.start && !lastFrameInputs_map.find(gamepadIdx)->second.start) {
        if (g_gameState.GetState() == DinoGameState::Pause)
            g_gameState.ChangeGameState(DinoGameState::Playing);
        else
            g_gameState.ChangeGameState(DinoGameState::Pause);
            }*/
    
    //updating dino_players
    /*if (gamepadDino_map.contains(gamepadIdx) && g_gameState.GetState() != DinoGameState::Pause) {
        gamepadDino_map.at(gamepadIdx).DinoPlayer_ReadGamePad(gamepad, deltaTime);
    }*/
    //
    
    //logic active only in game
    //adding an animal, max amount is 20
    /*if (g_gameState.GetState() == DinoGameState::Playing) {
        if (g_spawnTimer > std::lerp(g_endSpawnDelay, g_baseSpawnDelay, g_timeLeft / PLAYING_TIME) && animals.size() <
            50) {
            int index = animals.size();
            animals.emplace_back(g_terrainTopLeft, 8);
            g_dinoEntityManager.AddEntity(&animals[index]);
            g_dinoEntityManager.AddEntity(&animals[index]);
            g_spawnTimer = 0;
            }
        g_spawnTimer += deltaTime;
    }*/
    
    // Timer
    /*{
        if (g_gameState.GetState() == DinoGameState::Playing) {
            g_timeLeft -= deltaTime;
            g_timeLeft = std::max(g_timeLeft, 0.0f);
        }

        std::string text = std::format("{:04.1f}", g_timeLeft);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, {240 - textSize.x / 2, 0}, 2);
    }*/
    
    //drawing all entities
    /*if (g_gameState.GetState() != DinoGameState::Pause) {
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
    }*/
}

void PlayingState::ExitState()
{
}