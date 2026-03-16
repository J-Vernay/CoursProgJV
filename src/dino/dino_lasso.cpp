#include "dino_lasso.h"

#include "dino_draw_utils.h"
#include "dino_geometry.h"

dino_lasso::dino_lasso(dino_player& player)
{
    attachedPlayer = &player;
}

void dino_lasso::SimpleDrawLasso()
{
    std::vector<DinoVertex> vs;
    DinoColor lineColor =
        attachedPlayer->dinoID == 0
            ? DinoColor_BLUE
            : attachedPlayer->dinoID == 1
            ? DinoColor_RED
            : attachedPlayer->dinoID == 2
            ? DinoColor_YELLOW
            : DinoColor_GREEN;

    Dino_GenVertices_Polyline(vs, lassoPoints, 5, lineColor);
    DinoVertexBuffer lassoVertexBuffer(vs.data(), vs.size(), "lasso");
    XDino_Draw(lassoVertexBuffer.Get(), XDino_TEXID_WHITE, {}, 1);
}

void dino_lasso::UpdateLasso(std::vector<dino_Entity*>& entities, double timeSinceStart)
{
    DinoVec2 newPos = attachedPlayer->entityPosition;
    int playerId = attachedPlayer->dinoID;

    if (lassoPoints.size() <= 0 || LenghtSQR(newPos, lassoPoints.back()) > 0.0001f) {
        lassoPoints.push_back(newPos);
        lassoPointsTime.push_back(timeSinceStart);
    }

    //removing old vertices
    while (lassoPointsTime.size() > 0 && lassoPointsTime.front() < timeSinceStart - LASSO_POINT_LIVING_TIME) {
        lassoPoints.erase(lassoPoints.begin());
        lassoPointsTime.erase(lassoPointsTime.begin());
    }

    if (lassoPoints.size() < 4)
        return;

    DinoVec2 A = lassoPoints[lassoPoints.size() - 2];
    DinoVec2 B = lassoPoints[lassoPoints.size() - 1];

    for (int j = 0; j < lassoPoints.size() - 4; j++) {
        DinoVec2 C = lassoPoints[j];
        DinoVec2 D = lassoPoints[j + 1];
        if (ArePointsToClose(C, D))
            continue;
        if (Dino_IntersectSegment(A, B, C, D)) {
            for (dino_Entity* entity : entities) {
                if (IsPointInLoop(entity->entityPosition, j, lassoPoints.size() - 1)) {
                    entity->LassoCatched(attachedPlayer->dinoID);
                }
            }
            lassoPoints.erase(lassoPoints.begin() + j, lassoPoints.end());
            lassoPointsTime.erase(lassoPointsTime.begin() + j, lassoPointsTime.end());
            break;
        }
    }
    SimpleDrawLasso();
}

void dino_lasso::CutLasso(int fromIndex)
{
    lassoPoints.erase(lassoPoints.begin(), lassoPoints.begin() + fromIndex);
    lassoPointsTime.erase(lassoPointsTime.begin(), lassoPointsTime.begin() + fromIndex);
}

bool dino_lasso::ArePointsToClose(DinoVec2 p1, DinoVec2 p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return dx * dx + dy * dy < 0.0001f;
}

bool dino_lasso::IsPointInLoop(DinoVec2 p, int index1, int index2)
{
    DinoVec2 p2 = {1000, p.y};
    int intersectionCount = 0;

    for (int j = index1; j < index2; j++) {
        DinoVec2 C = lassoPoints[j];
        DinoVec2 D = lassoPoints[j + 1];
        if (ArePointsToClose(C, D))
            continue;
        if (Dino_IntersectSegment(p, p2, C, D)) {
            intersectionCount++;
        }
    }

    return intersectionCount % 2 != 0;
}

bool dino_lasso::IsMyPlayer(dino_player& player)
{
    return &player == attachedPlayer;
}

float dino_lasso::LenghtSQR(DinoVec2 p1, DinoVec2 p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}