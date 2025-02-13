#pragma once
#include "xdino.h"

#include <string>

class UIText
{
public:
    explicit UIText();
    explicit UIText(std::string text);
    explicit UIText(std::string text, DinoVec2 pos);

    void Update(float deltaTime);
    void Draw(DinoVec2 windowSize);
protected:
    std::string text;
    DinoVec2 pos;
};
