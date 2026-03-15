#pragma once

//forward declaration
class DinoGameState;

class GameState {

public:  
    virtual ~GameState() = default;
    
    virtual void EnterState() = 0;
    virtual void UpdateState(float deltaTime, double timeSinceStart) = 0;
    virtual void ExitState() = 0;

protected:
    DinoGameState* m_dinoGameState;
};