#include "DinoGameState.h"

DinoGameState::DinoGameState()
{

}

void DinoGameState::Update()
{
    if (!m_dinoTerrain.has_value())
        m_dinoTerrain.emplace(XDino_RandomInt32(0, 3));

    m_dinoTerrain->DinoTerrain_Draw();
}

DinoGameState::game_state DinoGameState::GetState()
{
    return currentGameState;
}

void DinoGameState::ChangeGameState(game_state newGameState)
{
    currentGameState = newGameState;
    switch (newGameState) {
    case Lobby: m_dinoTerrain.emplace(0);
        break;

    case Pause: break;

    case Playing: if (!m_dinoTerrain.has_value())
            m_dinoTerrain.emplace(0);
        break;
    }
}

void DinoGameState::Shutdown()
{
    m_dinoTerrain.reset();
}