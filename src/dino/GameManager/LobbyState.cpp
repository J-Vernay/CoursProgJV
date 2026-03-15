#include <dino/GameManager/LobbyState.h>
#include <dino/GameManager/DinoGameState.h>

LobbyState::LobbyState(DinoGameState* dino_game_state, int season)
{
    m_dinoGameState = dino_game_state;
    m_dinoTerrain.emplace(season);
}

void LobbyState::EnterState()
{
    texId_terrain = XDino_CreateGpuTexture("terrain.png");
    DinoVec2 offset = m_dinoGameState->g_terrainTopLeft;
    
    m_trees.emplace_back(offset+DinoVec2{50, 40}, 0, texId_terrain, m_dinoGameState);
    m_dinoGameState->g_dinoEntityManager.AddEntity(&m_trees.back());
    
    m_trees.emplace_back(offset+DinoVec2{100, 40}, 1, texId_terrain, m_dinoGameState);
    m_dinoGameState->g_dinoEntityManager.AddEntity(&m_trees.back());
    
    m_trees.emplace_back(offset+DinoVec2{150, 40}, 2, texId_terrain, m_dinoGameState);
    m_dinoGameState->g_dinoEntityManager.AddEntity(&m_trees.back());
    
    m_trees.emplace_back(offset+DinoVec2{200, 40}, 3, texId_terrain, m_dinoGameState);
    m_dinoGameState->g_dinoEntityManager.AddEntity(&m_trees.back());
}

void LobbyState::UpdateState(float deltaTime, double timeSinceStart)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        //DEBUG ONLY
        /*if (gamepad.btn_up) {

            g_gameState.ChangeGameState(std::make_unique<PlayingState>());
        }*/
        
        //lastFrameInputs_map[gamepadIdx] = gamepad;
        
        //spawning dino_players
        if (!m_dinoGameState->gamepadDino_map.contains(gamepadIdx)
            && (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
            //max player amount == 4
            && m_dinoGameState->gamepadDino_map.size() < 4) {

            dino_player newPlayer = dino_player(
                {m_dinoGameState->g_terrainTopLeft.x + 128, m_dinoGameState->g_terrainTopLeft.y + 96},
                8,
                m_dinoGameState->texID_dino,
                m_dinoGameState->gamepadDino_map.size(),
                m_dinoGameState->g_terrainTopLeft);

            auto [it,bInserted] = m_dinoGameState->gamepadDino_map.emplace(gamepadIdx, newPlayer);
            m_dinoGameState->g_dinoEntityManager.AddEntity(&it->second);
            m_dinoGameState->g_lassoManager.AddLasso(it->second);
            }
        
        //despawning player
        if (m_dinoGameState->gamepadDino_map.contains(gamepadIdx)
            && gamepad.select) {

        dino_player& player = m_dinoGameState->gamepadDino_map.find(gamepadIdx)->second;

        m_dinoGameState->g_dinoEntityManager.RemoveEntity(player);        

        m_dinoGameState->g_lassoManager.RemovePlayer(player);

        m_dinoGameState->gamepadDino_map.find(gamepadIdx)->second.DinoPlayer_Despawn();
        m_dinoGameState->gamepadDino_map.erase(gamepadIdx);
        }
        
        //updating dino_players
        if (m_dinoGameState->gamepadDino_map.contains(gamepadIdx)) {
            m_dinoGameState->gamepadDino_map.at(gamepadIdx).DinoPlayer_ReadGamePad(gamepad, deltaTime);
        }
        //
    }
    
    //drawing terrain
    /*if (!m_dinoTerrain.has_value())
        m_dinoTerrain.emplace(XDino_RandomInt32(0, 3));*/
    m_dinoTerrain->DinoTerrain_Draw();
    
    //drawing all entities
    m_dinoGameState->g_lassoManager.UpdateLassos(m_dinoGameState->g_dinoEntityManager.entities);
    m_dinoGameState->g_dinoEntityManager.DrawEntitiesWithTerrainClamping(timeSinceStart, m_dinoGameState->g_terrainTopLeft);
    //resolving all collisions between entities
    m_dinoGameState->g_dinoEntityManager.DinoCollision_HandleCollisions(m_dinoGameState->g_terrainTopLeft);
    
    for(dino_tree& tree : m_trees){
        tree.DrawEntity(timeSinceStart);
    }
}

void LobbyState::ExitState()
{
    for(dino_tree& tree : m_trees) {
        m_dinoGameState->g_dinoEntityManager.RemoveEntity(tree);
    }
    
    XDino_DestroyGpuTexture(texId_terrain);
    m_dinoTerrain.reset();
}