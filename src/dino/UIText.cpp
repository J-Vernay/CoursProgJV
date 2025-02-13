#include "UIText.h"
#include "dino_draw_utils.h"

UIText::UIText()
{
    text = "";
    pos = {0, 0};
}

UIText::UIText(std::string text)
{
    this->text = text;
    pos = {0, 0};
}

UIText::UIText(std::string text, DinoVec2 pos)
{
    this->text = text;
    this->pos = pos;
}

void UIText::Update(const float deltaTime)
{
    
}

void UIText::Draw(const DinoVec2 windowSize)
{
    DinoVec2 size;
    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_ORANGE, &size);
    drawCall.scale = 2;
    drawCall.translation = pos;
    XDino_Draw(drawCall);
}
