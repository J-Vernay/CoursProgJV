#pragma once
#include <dino/xdino.h>
#include <dino/dino_game.h>

void Pause_Update(bool bPressedStart, bool& bWasStartPressed, EGameState& state);
void Pause_Draw(double timeSinceStart);