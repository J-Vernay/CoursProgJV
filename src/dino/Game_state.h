#pragma once


class DinoGameState;

class GameState {

public:
    virtual ~GameState() = default;
    virtual void EnterState(double timeSinceStart) = 0;
    virtual void UpdateState(float deltaTime, double timeSinceStart) = 0;
    virtual void ExitState() = 0;

protected:
    DinoGameState* m_dinoGameState;
};