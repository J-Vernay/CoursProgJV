#pragma once
#include "UIText.h"

class UIName : public UIText
{
public:
    explicit UIName();
    explicit UIName(std::string text);
    
    void Draw(DinoVec2 windowSize);
};
