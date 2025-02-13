#include <dino/dino_lasso.h>

constexpr float LINE_WIDTH = 4;

void DinoLasso::update(float deltaTime, DinoVec2 newPosition)
{
    updateAddPoints(newPosition);
}

void DinoLasso::draw(DinoColor color) const
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(linePoints, LINE_WIDTH, color);
    XDino_Draw(drawCall);
}

void DinoLasso::updateAddPoints(DinoVec2 newPosition)
{
    if (abs(position.x - newPosition.x) < std::numeric_limits<float>::epsilon() && abs(position.y - newPosition.y) < std::numeric_limits<float>::epsilon()) return;
    linePoints.insert(linePoints.begin(), newPosition);
}