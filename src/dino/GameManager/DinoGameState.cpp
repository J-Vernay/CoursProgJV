#include <dino/GameManager/DinoGameState.h>


void DinoGameState::Init(double timeSinceStart)
{
    currentGameState = std::make_unique<LobbyState>(this, XDino_RandomInt32(0, 3));
    currentGameState->EnterState(timeSinceStart);

    g_scoreManager.ResetScores(gamepadDino_map);

    // Préparation des textures.
    {
        texID_dino = XDino_CreateGpuTexture("dinosaurs.png");
    }
}

void DinoGameState::Update(float deltaTime, double timeSinceStart)
{
    if (nextState) {
        if (currentGameState)
            currentGameState->ExitState();
        currentGameState = std::move(nextState);
        currentGameState->EnterState(timeSinceStart);
    }

    currentGameState->UpdateState(deltaTime, timeSinceStart);
    g_scoreManager.DrawScores();
}


void DinoGameState::ChangeGameState(std::unique_ptr<GameState> newGameState)
{
    nextState = std::move(newGameState);
}

void DinoGameState::Shutdown()
{
    currentGameState->ExitState();
    XDino_DestroyGpuTexture(texID_dino);
}