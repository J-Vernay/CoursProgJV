#pragma once

#include <optional>
#include <dino/dino_terrain.h>

class DinoGameState {

public:
    DinoGameState();

    enum game_state { Lobby, Pause, Playing };

    void Update();
    game_state GetState();
    void ChangeGameState(game_state newGameState);
    void Shutdown();

private:
    game_state currentGameState = Lobby;
    std::optional<dino_terrain> m_dinoTerrain;

};