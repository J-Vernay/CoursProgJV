/// @file dino_lasso.cpp
/// @brief Implémentation de `DinoLasso`.

#include <functional>
#include <dino/dino_player.h>
#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_lasso.h>

constexpr float LINE_WIDTH = 4;

void DinoLasso::update(float deltaTime, DinoActor* actor)
{
    handleAddingPoints(actor->position);
    handleRemovingPoints(deltaTime);
}

void DinoLasso::draw(DinoColor color) const
{
    XDino_ProfileBegin(DinoColor_BLUE, "Draw lasso");
    DinoDrawCall drawCall = Dino_CreateDrawCall_Polyline(linePoints, LINE_WIDTH, color);
    XDino_Draw(drawCall);
    XDino_ProfileEnd();
}

void DinoLasso::handleActorCollision(DinoActor* actor)
{
    if (linePoints.size() <= 2)
        return;
    auto point = linePoints.begin();

    while (point < linePoints.end() - 1) {
        if (!Dino_IntersectSegment(actor->position, actor->lastPosition, point[0], point[1])) {
            ++point;
            continue;
        }

        deleteTimer -= static_cast<float>(linePoints.end() - point) / 60.0f;
        deleteTimer = std::max(deleteTimer, 0.0f);
        linePoints.erase(point, linePoints.end());
        break;
    }
}

void DinoLasso::handleAddingPoints(DinoVec2 newPosition)
{
    linePoints.insert(linePoints.begin(), newPosition);
}

void DinoLasso::handleRemovingPoints(float deltaTime)
{
    deleteTimer += deltaTime;
    if (deleteTimer < 120.5f / 60.0f)
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

void DinoLasso::handleSelfIntersection(const intersection_callback& callback)
{
    if (linePoints.size() < 4)
        return;

    auto point = linePoints.begin() + 2;

    while (point < linePoints.end() - 1) {
        if (!Dino_IntersectSegment(linePoints.begin()[0], linePoints.begin()[1], point[0], point[1])) {
            ++point;
            continue;
        }

        if (callback != nullptr) {
            callback(linePoints.begin(), point + 1);
        }

        deleteTimer -= static_cast<float>(point - linePoints.begin() - 1) / 60.0f;
        deleteTimer = std::max(deleteTimer, 0.0f);
        linePoints.erase(linePoints.begin() + 2, point + 1);
        break;
    }
}