#include "UIFrame.h"

#include <format>

void UIFrame::Update(const float deltaTime)
{
    tex = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
}

