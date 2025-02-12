#pragma once

#include <dino/xdino.h>

class DinoPlayer
{
    
public:
    void UpdatePlayer(float deltaTime);
    void DisplayPlayer(double timeSinceStart);
    void SetPlayerPos(DinoVec2 pos);
    
private:
    DinoVec2 player_pos_ = {};
    bool mirror_ = false;

    bool d_idle_ = false;
    bool d_walking_ = false;
    bool d_running_ = false;

    int spriteIdx = 0;
};
