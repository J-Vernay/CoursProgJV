#include "UIName.h"

#include "dino_draw_utils.h"

UIName::UIName()
{
    text = "";
    pos = {0, 0};
}

UIName::UIName(std::string text)
{
    this->text = text;
    pos = {0, 0};
}

void UIName::Draw(const DinoVec2 windowSize)
{
    DinoVec2 size;
    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_ORANGE, &size);
    drawCall.scale = 2;
    drawCall.translation = {windowSize.x - size.x * 2, windowSize.y - size.y * 2};
    XDino_Draw(drawCall);
}

