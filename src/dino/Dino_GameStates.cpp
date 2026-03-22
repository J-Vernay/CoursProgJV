#include <dino/Dino_GameStates.h>


void DinoGameState::Init(double timeSinceStart)
{

    DinoPlayer::InitStatic();
    g_scoreManager.ResetScores();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        unassignedGamepads.emplace_back(idx);

    currentState = std::make_unique<LobbyState>(this, XDino_RandomInt32(0, 3));
    currentState->EnterState(timeSinceStart);
}

void DinoGameState::Update(float deltaTime, double timeSinceStart)
{
    if (nextState) {
        if (currentState)
            currentState->ExitState();

        currentState = std::move(nextState);
        currentState->EnterState(timeSinceStart);
    }

    currentState->UpdateState(deltaTime, timeSinceStart);
    currentState->DrawState(deltaTime, timeSinceStart);

}

void DinoGameState::ChangeState(std::unique_ptr<GameState> newState, double timeSinceStart)
{
    nextState = std::move(newState);
}

void DinoGameState::Shut()
{
    currentState->ExitState();
    DinoPlayer::ShutStatic();
}