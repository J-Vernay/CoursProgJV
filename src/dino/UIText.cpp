#include "UIText.h"
#include "dino_draw_utils.h"

UIText::UIText()
{
    tex = "";
    pos = {0, 0};
    windowSize = {0, 0};
}

UIText::UIText(const std::string& tex_d, const DinoVec2 windowSize) : Entity(tex_d)
{
    this->tex = tex_d;
    pos = {0, 0};
    this->windowSize = windowSize;
}

UIText::UIText(const std::string& tex_d, const DinoVec2 pos_d, const DinoVec2 windowSize) : Entity(tex_d, pos_d)
{
    this->tex = tex_d;
    this->pos = pos_d;
    this->windowSize = windowSize;
}

void UIText::Update(const float deltaTime)
{
    
}

void UIText::Draw()
{
    DinoVec2 size;
    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(tex, DinoColor_WHITE, DinoColor_ORANGE, &size);
    drawCall.scale = 2;
    drawCall.translation = pos;
    XDino_Draw(drawCall);
}
