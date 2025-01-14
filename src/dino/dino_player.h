#pragma once

#include <dino/xdino.h>

class DinoPlayer {
public:
    void Init(DinoVec2 initPos, DinoGamepadIdx idxGamepad_, int32_t idxPlayer_);
    void Update(double timeSinceStart, float deltaTime);

    static const std::string drawCallTextureName;

    void Draw(double timeSinceStart, float deltaTime, DinoDrawCall& drawCall);

private:
    DinoVec2 dinoPos{};
    DinoVec2 dinoMove{};
    bool bDinoLeft = false;
    bool bDinoRunning = false;
    bool bDinoDamage = false;
    double dinoDamageStart = 0;
    DinoGamepadIdx idxGamepad{};
    int32_t idxPlayer{};
};