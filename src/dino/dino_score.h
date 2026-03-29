#pragma once
#include <dino/xdino.h>
#include <dino/dino_game.h>
#include <vector>

struct ScoreNotification {
    DinoVec2 pos;
    int points;
    DinoColor color;
    double spawnTime;
};

void Score_Draw(std::vector<PlayerState>& players, double timeSinceStart);
void Score_AddNotification(DinoVec2 pos, int points, DinoColor color, double timeSinceStart);