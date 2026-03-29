#pragma once
#include "dino_lasso.h"
#include "dino_player.h"

#include <dino/xdino.h>
#include <vector>
#include <dino/dino_game.h>

class DinoTerrain;
class DinoTree;
class DinoEntity;

void Lobby_Update(double timeSinceStart, float deltaTime,
                  std::vector<PlayerState>& players,
                  std::vector<DinoGamepadIdx>& unassignedGamepads,
                  std::vector<DinoTree>& trees,
                  std::vector<DinoEntity*>& entities,
                  DinoTerrain& terrain,
                  EGameState& state);

void Lobby_Draw(double timeSinceStart, std::vector<PlayerState>& players,
                std::vector<DinoEntity*>& entities);