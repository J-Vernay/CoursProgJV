#pragma once
#include "Entity.h"
#include "xdino.h"

#include <string>

class UIText : public Entity
{
public:
    explicit UIText();
    explicit UIText(const std::string& tex_d, DinoVec2 windowSize);
    explicit UIText(const std::string& tex_d, DinoVec2 pos_d, DinoVec2 windowSize);

    void Update(float deltaTime) override;
    void Draw() override;
    
protected:
    DinoVec2 windowSize;
};
