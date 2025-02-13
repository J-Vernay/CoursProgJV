#pragma once
#include <string>

#include "dino/xdino.h"

class Dino
{
public:
    explicit Dino();
    explicit Dino(DinoGamepadIdx gamepad_idx, const std::string& tex, uint16_t id);
    explicit Dino(DinoGamepadIdx gamepad_idx, const std::string& tex, uint16_t id, DinoVec2 pos);

    static bool CompareHeight(const Dino& d1, const Dino& d2);
    
    void Update(float deltaTime);
    
    void Draw() const;
    
private:
    std::string tex;
    DinoVec2 pos;
    DinoGamepadIdx gamepad_idx;
    
    uint16_t id;

    uint16_t i;
    uint16_t j;

    uint16_t k;
    uint16_t f;
    float animationTime;

    bool inverted;
    bool stunned;
    
    void SetUV(float deltaTime, uint16_t offset);
};
