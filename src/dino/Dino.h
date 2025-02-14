#pragma once
#include "Entity.h"

#include <string>

#include "dino/xdino.h"

class Dino : public Entity
{
public:
    explicit Dino();
    explicit Dino(DinoGamepadIdx gamepad_idx, const std::string& tex_d, uint16_t id);
    explicit Dino(DinoGamepadIdx gamepad_idx, const std::string& tex_d, uint16_t id, DinoVec2 pos_d);

    static bool CompareHeight(const Dino* d1, const Dino* d2);
    
    void Update(float deltaTime) override;
    
    void Draw() override;
    
private:
    DinoGamepadIdx gamepad_idx;
    
    uint16_t id;

    uint16_t i;
    uint16_t j;

    uint16_t k;
    uint16_t f;
    float animationTime;

    bool inverted;
    bool stunned;

    bool _active;
    
    void SetUV(float deltaTime, uint16_t offset);
};
