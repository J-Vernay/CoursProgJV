#pragma once
#include <dino/xdino.h>
#include <vector>

enum class EGameState;
struct PlayerState;

enum class EPauseAction { None, Resume, Restart, BackToLobby };

void Pause_Update(std::vector<PlayerState>& players,
                  double& chrono,
                  EGameState& state,
                  EPauseAction& action);

void Pause_Draw(double timeSinceStart, double chrono);