#include "DinoGameState.h"

DinoGameState::DinoGameState()
{
    ChangeGameState(Lobby);
}

DinoGameState::~DinoGameState()
{
    m_dinoTerrain.emplace(0);
}

void DinoGameState::Update()
{
    m_dinoTerrain->DinoTerrain_Draw();
}

void DinoGameState::ChangeGameState(game_state newGameState)
{
    currentGameState = newGameState;
    switch (newGameState) {
        case Lobby:
        m_dinoTerrain.emplace(0);        
        break;

        case Pause:
        break;

        case Season1:
        m_dinoTerrain.emplace(0);
        break;

        case Season2:
        m_dinoTerrain.emplace(1);
        break;

        case Season3:
        m_dinoTerrain.emplace(2);
        break;

        case Season4:
        m_dinoTerrain.emplace(3);
        break;
    }
}

DinoVec2 DinoGameState::GetTopLeft()
{
    return m_dinoTerrain->DinoTerrain_GetTopLeft();
}