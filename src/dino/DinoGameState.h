#pragma once

#include <optional>
#include <dino/dino_terrain.h>

class DinoGameState {
    
public:
    DinoGameState();
    ~DinoGameState();
    
    enum game_state {Lobby, Pause, Season1, Season2, Season3, Season4 };
    void Update();
    void ChangeGameState(game_state newGameState);
    DinoVec2 GetTopLeft();
    
private:
    game_state currentGameState = Lobby;
    std::optional<dino_terrain> m_dinoTerrain;
    
};