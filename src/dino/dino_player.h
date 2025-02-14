#pragma once

#include <dino/xdino.h>
#include <dino/dino_entity.h>

class DinoPlayer : public DinoEntity
{
    
public:
    void Update(float deltaTime) override;
    void Display(double timeSinceStart ) override;
    void DrawLasso();
    void DinoPlayerInit(DinoVec2 pos, int idx, DinoGamepadIdx gamepadIdx, DinoColor lassoColor);
    void SetPlayerIdx(int idx);
    
private:
    bool mirror_ = false;

    bool d_idle_ = false;
    bool d_walking_ = false;
    bool d_running_ = false;

    int sprite_idx_ = 0;
    int idx_player_ = 0;
    DinoGamepadIdx idx_gamepad_;

    DinoColor lassoColor_;
    std::vector<DinoVec2> lasso;
};
