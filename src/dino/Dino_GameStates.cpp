#include <dino/dino_GameStates.h>
#include <dino/Lobby_state.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>

void DinoGameState::Init()
{
    DinoPlayer::InitStatic();
    DinoTree::InitStatic();
    spawner.Init(scoreManager);
    scoreManager.ResetScores();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        unassignedGamepads.emplace_back(idx);

    int idxSeason = XDino_RandomInt32(0, 3);
    terrain.Init(RENDER_SIZE, idxSeason);

    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    //TransitionTo(std::make_unique<LobbyState>(), 0.0);
}

void DinoGameState::Shut()
{
    if (m_currentState)
        m_currentState->ExitState();

    for (PlayerState& ps : players)
        ps.player.Shut();

    XDino_DestroyVertexBuffer(m_vbuffID_nom);
    spawner.Shut();
    terrain.Shut();
}

void DinoGameState::Frame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - m_lastTime);
    m_lastTime = timeSinceStart;

    XDino_SetRenderSize(RENDER_SIZE);

    if (m_currentState)
        m_currentState->UpdateState(deltaTime, timeSinceStart);

}

void DinoGameState::TransitionTo(std::unique_ptr<GameState> newState, double timeSinceStart)
{
    if (m_currentState)
        m_currentState->ExitState();

    m_currentState = std::move(newState);
    //m_currentState->m_dinoGameState = this;
    m_currentState->EnterState(timeSinceStart);
}