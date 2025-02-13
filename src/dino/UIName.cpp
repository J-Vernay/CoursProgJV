#include "UIName.h"

#include "dino_draw_utils.h"

UIName::UIName()
{
    tex = "";
    pos = {0, 0};
}

UIName::UIName(const std::string& text_d, const DinoVec2 windowSize_d) : UIText(text_d, windowSize_d)
{
    this->tex = text_d;
    pos = {0, 0};
}

void UIName::Draw() const
{
    DinoVec2 size;
    DinoDrawCall drawCall = Dino_CreateDrawCall_Text(tex, DinoColor_WHITE, DinoColor_ORANGE, &size);
    drawCall.scale = 2;
    drawCall.translation = {windowSize.x - size.x * 2, windowSize.y - size.y * 2};
    XDino_Draw(drawCall);
}

