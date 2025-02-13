#include "PolyLine.h"

#include "dino_draw_utils.h"

PolyLine::PolyLine()
{
    polyline.resize(1);
}

PolyLine::PolyLine(const int size)
{
    polyline.resize(size);
}

void PolyLine::Update(const float deltaTime)
{
    /*polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
    polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
    polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);*/
}

void PolyLine::Draw()
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(polyline, 100, POLYLINE_COLOR);
    XDino_Draw(drawCall);
}
