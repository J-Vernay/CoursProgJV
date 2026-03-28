#pragma once

#include "dino_gamestate.h"

#include <dino/dino_animal.h>
#include <vector>

struct ScoreNotif {
    int       points;
    DinoVec2  position;
    DinoColor color;
    double    spawnTime;
};

class PlayState : public GameState {
public:
    explicit PlayState(double timeSinceStart);

    GameState* Update(double timeSinceStart, float deltaTime, GameContext& ctx) override;
    void       Draw(double timeSinceStart, const GameContext& ctx) const override;

private:
    void UpdatePauseMenu(float deltaTime, GameContext& ctx);
    void UpdateGameplay(double timeSinceStart, float deltaTime, GameContext& ctx);
    void DrawPauseMenu(const GameContext& ctx) const;
    void DrawScoreNotifs(double timeSinceStart) const;

    std::vector<DinoAnimal>   m_animals;
    std::vector<ScoreNotif>   m_scoreNotifs;

    double m_chrono          = 15.0;
    double m_timeSpawnAnimal = 0.0;

    bool m_bPause           = false;
    bool m_bWasStartPressed = false;
    bool m_bWasDpadPressed  = false;
    int  m_pauseMenuIdx     = 0;

    static constexpr double CHRONO_INIT    = 15.0;
    static constexpr double SPAWNTIME_BEGIN = 1.0;
    static constexpr double SPAWNTIME_END   = 0.033;
};