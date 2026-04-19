#pragma once

#include <dino/xdino.h>
#include <string>

class FloatingText
{
public:
    FloatingText(DinoVec2 spawnPosition, std::string displayText, DinoColor textColor, float lifetime = 1.5f);

    bool IsExpired() const;
    void Update(float deltaTime);
    void Draw() const;

private:
    DinoVec2 currentPosition;
    std::string displayText;
    DinoColor textColor;
    float lifetime;
    float elapsed = 0.f;
};