#include <dino/dino_lasso.h>

constexpr float LINE_WIDTH = 4;

void DinoLasso::update(float deltaTime, DinoVec2 newPosition)
{
    updateAddPoints(newPosition);
    updateRemovePoints(deltaTime);
}

void DinoLasso::draw(DinoColor color) const
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(linePoints, LINE_WIDTH, color);
    XDino_Draw(drawCall);
}

void DinoLasso::updateAddPoints(DinoVec2 newPosition)
{
    linePoints.insert(linePoints.begin(), newPosition);
}

void DinoLasso::updateRemovePoints(float deltaTime)
{
    deleteTimer += deltaTime;
    if (deleteTimer < 122 / 60.0f)
        return;

    if (linePoints.empty()) {
        deleteTimer = 0;
    }

    do {
        linePoints.pop_back();

        if (linePoints.empty()) {
            deleteTimer = 0;
            return;
        }

        deleteTimer -= 1 / 60.0f;
    } while (deleteTimer < 1 / 60.0f);
}