#include "Lobby_state.h"

#include <dino/Dino_GameStates.h>
#include <string>
#include <iostream>


void DinoGameState::Init(double timeSinceStart)
{

    DinoPlayer::InitStatic();
    DinoTree::InitStatic();
    g_scoreManager.ResetScores();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        unassignedGamepads.emplace_back(idx);

    {
        std::vector<DinoVertex> vs;
        m_textSize_nom = Dino_GenVertices_Text(vs, "Betremieux Elouan", DinoColor_WHITE, DinoColor_GREY);
        m_vbuffID_nom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

    currentState = std::make_unique<LobbyState>(this, XDino_RandomInt32(0, 3));
    currentState->EnterState(timeSinceStart);

}

void DinoGameState::DrawState(float deltaTime, double timeSinceStart)
{
}

void DinoGameState::Update(float deltaTime, double timeSinceStart)
{
    if (nextState) {
        if (currentState)
            currentState->ExitState();

        currentState = std::move(nextState);
        nextState = nullptr;
        currentState->EnterState(timeSinceStart);
    }

    currentState->UpdateState(deltaTime, timeSinceStart);
    if (nextState)
        return;
    currentState->DrawState(deltaTime, timeSinceStart);

}

void DinoGameState::ChangeState(std::unique_ptr<GameState> newState, double timeSinceStart)
{
    nextState = std::move(newState);
}

void DinoGameState::Shut()
{
    XDino_DestroyVertexBuffer(m_vbuffID_nom);
    currentState->ExitState();
    DinoPlayer::ShutStatic();
}