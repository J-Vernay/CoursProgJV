#pragma once
#include <dino/xdino.h>
#include <vector>
#include <dino/dino_game.h>

enum class EGameState;
struct PlayerState;
class DinoAnimal;
class DinoEntity;
class DinoTerrain;

void InGame_Update(double timeSinceStart, float deltaTime,
                   std::vector<PlayerState>& players,
                   std::vector<DinoAnimal>& animals,
                   std::vector<DinoEntity*>& entities,
                   DinoTerrain& terrain,
                   double& chrono,
                   double& timeSpawnAnimal,
                   bool bPressedStart,
                   bool& bWasStartPressed,
                   EGameState& state);

void InGame_Draw(double timeSinceStart,
                 std::vector<PlayerState>& players,
                 std::vector<DinoEntity*>& entities);