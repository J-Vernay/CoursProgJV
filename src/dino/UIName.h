#pragma once
#include "UIText.h"

class UIName : public UIText
{
public:
    explicit UIName();
    explicit UIName(const std::string& text_d, DinoVec2 windowSize_d);
    
    void Draw() const;
};
