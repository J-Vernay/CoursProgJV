#pragma once

#include "game_state.h"
#include <dino/Dino_GameStates.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/xdino.h>


class PlayState : public GameState {

public:
    PlayState(DinoGameState* dinoGameState, int season);

    void EnterState(double timeSinceStart) override;
    void UpdateState(float deltaTime, double timeSinceStart) override;
    void DrawState(float deltaTime, double timeSinceStart) override;
    void ExitState() override;

private:
    void UpdatePauseInput();
    void DrawPauseMenu();

    DinoTerrain m_terrain;
    int m_season;

    double m_chrono = 60.0;
    bool m_paused = false;
    int m_currentPauseButton = 3;

    bool m_bWasStartPressed = false;
    bool m_bWasPauseUpPressed = false;
    bool m_bWasPauseDownPressed = false;
    bool m_bWasPauseLeftPressed = false;
    bool m_bWasPauseRightPressed = false;

    std::vector<DinoEntity*> m_entities;
};