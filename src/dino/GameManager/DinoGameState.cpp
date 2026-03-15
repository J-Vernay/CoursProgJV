#include <dino/GameManager/DinoGameState.h>

#include "LobbyState.h"


void DinoGameState::Init()
{
    currentGameState = std::make_unique<LobbyState>(this, XDino_RandomInt32(0,3));
    currentGameState->EnterState();
    
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
        currentGameState->EnterState();
    }
    
    currentGameState->UpdateState(deltaTime, timeSinceStart);
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