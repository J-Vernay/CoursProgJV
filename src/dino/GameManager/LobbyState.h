#pragma once


#include <optional>
#include <dino/dino_geometry.h>
#include <dino/GameManager/GameState.h>
#include <dino/GameManager/dino_terrain.h>
#include <dino/Entities/dino_tree.h>
#include <dino/Entities/dino_player.h>


class LobbyState : public GameState {

public:
    LobbyState(DinoGameState* dino_game_state, int season);
    
    uint64_t texId_terrain;
    
    void EnterState() override;
    void UpdateState(float deltaTime, double timeSinceStart) override;
    void ExitState() override;

private:
    std::deque<dino_tree> m_trees;
    std::optional<dino_terrain> m_dinoTerrain;
};