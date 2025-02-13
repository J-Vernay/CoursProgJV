#pragma once

#include <dino/xdino.h>

class DinoPlayer
{
    
public:
    void UpdatePlayer(float deltaTime);
    void DisplayPlayer(double timeSinceStart);
    void DinoPlayerInit(DinoVec2 pos, int idx, DinoGamepadIdx gamepadIdx);
    void SetPlayerPos(DinoVec2 pos);
    void SetPlayerIdx(int idx);
    bool IsAbove(DinoPlayer& other);
    
private:
    DinoVec2 player_pos_ = {};
    bool mirror_ = false;

    bool d_idle_ = false;
    bool d_walking_ = false;
    bool d_running_ = false;

    int sprite_idx_ = 0;
    int idx_player_ = 0;
    DinoGamepadIdx idx_gamepad_;
};
