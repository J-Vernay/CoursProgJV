#pragma once
#include <dino/GameManager/GameState.h>


class PlayingState : public GameState {

public:
    PlayingState(DinoGameState* dino_game_state, int Season);
    
    void EnterState() override;
    void UpdateState(float deltaTime, double timeSinceStart) override;
    void ExitState() override;

private:
    

};