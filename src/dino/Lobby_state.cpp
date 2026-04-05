#include "Playing_state.h"

#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <dino/Dino_GameStates.h>
#include <dino/Lobby_state.h>
#include <dino/dino_tree.h>


LobbyState::LobbyState(DinoGameState* dino_gameState, int season)
{
    m_dinoGameState = dino_gameState;
    m_season = season;
}

void LobbyState::EnterState(double timeSinceStart)
{
    m_terrain.Init(DinoGameState::RENDER_SIZE, m_season);
    DinoVec2 terrainMin = m_terrain.GetTopLeft();
    DinoVec2 terrainMax = m_terrain.GetBottomRight();
    for (int i = 0; i < 4; ++i) {
        float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
        float y = terrainMin.y + 80;
        m_trees.emplace_back(DinoVec2{x, y}, i);
        m_trees[i].canStartGame = !m_dinoGameState->gamePreviouslyEnded;
    }
}

void LobbyState::UpdateState(float deltaTime, double timeSinceStart)
{
    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad))
            player.gamepad = gamepad;
    }

    for (int i = 0; i < m_dinoGameState->unassignedGamepads.size(); i++) {
        DinoGamepadIdx idx = m_dinoGameState->unassignedGamepads[i];
        DinoGamepad gamepad;
        if (XDino_GetGamepad(idx, gamepad)) {
            if (gamepad.start) {
                if (!m_dinoGameState->freePlayerIndices.empty()) {
                    int idxPlayer = m_dinoGameState->freePlayerIndices.front();
                    m_dinoGameState->freePlayerIndices.erase(m_dinoGameState->freePlayerIndices.begin());
                    m_dinoGameState->g_players.emplace_back(idx,
                                                            gamepad,
                                                            idxPlayer,
                                                            DinoLasso(
                                                                DinoGameState::PLAYER_COLORS[idxPlayer],
                                                                idxPlayer));
                    m_dinoGameState->assignedGamepads.emplace_back(idx);
                    m_dinoGameState->unassignedGamepads.erase(m_dinoGameState->unassignedGamepads.begin() + i);
                    m_dinoGameState->g_scoreManager.AddPlayer(idxPlayer);
                }
                break;
            }

        }

    }

    for (int i = 0; i < m_dinoGameState->assignedGamepads.size(); i++) {
        DinoGamepadIdx idx = m_dinoGameState->assignedGamepads[i];
        DinoGamepad gamepad;
        if (XDino_GetGamepad(idx, gamepad)) {
            if (gamepad.select) {
                auto it = std::find_if(m_dinoGameState->g_players.begin(),
                                       m_dinoGameState->g_players.end(),
                                       [idx](DinoGameState::PlayerState& ps) {
                                           return ps.gamepadIdx == idx;
                                       });
                if (it != m_dinoGameState->g_players.end()) {
                    m_dinoGameState->g_scoreManager.RemovePlayer(it->player.m_idxPlayer);
                    m_dinoGameState->freePlayerIndices.push_back(it->player.m_idxPlayer);
                    std::sort(m_dinoGameState->freePlayerIndices.begin(), m_dinoGameState->freePlayerIndices.end());
                    m_dinoGameState->g_players.erase(it);
                }
                m_dinoGameState->unassignedGamepads.emplace_back(idx);
                m_dinoGameState->assignedGamepads.erase(m_dinoGameState->assignedGamepads.begin() + i);
                break;
            }
        }
    }
    m_entities.clear();
    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        player.player.Update(timeSinceStart, deltaTime, m_terrain, player.gamepad);

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players) {
        m_entities.push_back(&player.player);
    }
    for (DinoTree& tree : m_trees)
        m_entities.emplace_back(&tree);

    for (size_t idxA = 0; idxA < m_entities.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < m_entities.size(); ++idxB) {
            DinoEntity::ResolveCollision(*m_entities[idxA], *m_entities[idxB]);
        }

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        player.lasso.Update(player.player.GetPos());

    for (size_t idxA = 0; idxA < m_dinoGameState->g_players.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < m_dinoGameState->g_players.size(); ++idxB)
            DinoLasso::ResolveCollision(m_dinoGameState->g_players[idxA].lasso, m_dinoGameState->g_players[idxB].lasso);

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        for (DinoEntity* pEntity : m_entities)
            if (player.lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart, player.lasso.m_ownerIndex);

    m_terrain.Update(timeSinceStart);

    for (DinoTree& tree : m_trees) {
        tree.Update(timeSinceStart, deltaTime);
        if (tree.WasLooped() && tree.canStartGame) {
            m_trees.clear();
            m_dinoGameState->ChangeState(
                std::make_unique<PlayState>(m_dinoGameState, tree.GetIdxSeason()),
                timeSinceStart);
            break;

        }
    }

}

void LobbyState::DrawState(float deltaTime, double timeSinceStart)
{
    //Draw

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    // Affichage
    DinoVec2 renderSize = XDino_GetRenderSize();

    //Affichage de terrain
    m_terrain.Draw(timeSinceStart);

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        player.lasso.Draw();

    // Affichage du nom
    {
        XDino_Draw(m_dinoGameState->m_vbuffID_nom,
                   XDino_TEXID_FONT,
                   {renderSize.x - m_dinoGameState->m_textSize_nom.x * 2,
                    renderSize.y - m_dinoGameState->m_textSize_nom.y * 2},
                   2);
    }

    m_dinoGameState->g_scoreManager.DrawScores(m_terrain);
    std::sort(m_entities.begin(), m_entities.end(), DinoEntity::CompareVerticalPos);

    for (DinoEntity* pEntity : m_entities)
        pEntity->Draw(timeSinceStart);

    std::vector<DinoVertex> vs;
    DinoVec2 textSize = Dino_GenVertices_Text(vs, "Choose a tree to start", DinoColor_WHITE, DinoColor_GREY);
    DinoVertexBuffer vertex_buffer = {vs.data(), vs.size(), "Title"};
    float tx = (DinoGameState::RENDER_SIZE.x - textSize.x * 2) / 2;
    XDino_Draw(vertex_buffer.GetVbufID(),
               XDino_TEXID_FONT,
               {tx, 0},
               2);
}

void LobbyState::ExitState()
{
    m_terrain.Shut();
    m_trees.clear();
    m_entities.clear();
}