#include "dino_lasso.h"

#include "dino_draw_utils.h"
#include "dino_geometry.h"
#include "x64-windows/xdino_win64_rdr.h"

dino_lasso::dino_lasso(dino_player& player)
{
    attachedPlayer = &player;
}

void dino_lasso::UpdateLasso(std::vector<dino_Entity*>& entities)
{
    DinoVec2 newPos = attachedPlayer->entityPosition;
    int playerId = attachedPlayer->dinoID;

    lassoPoints.push_back(newPos);

    //removing old vertices
    if (lassoPoints.size() > 120)
        lassoPoints.erase(lassoPoints.begin(), lassoPoints.begin() + lassoPoints.size() - 120);

    if (lassoPoints.size() < 4)
        return;

    DinoVec2 A = lassoPoints[lassoPoints.size() - 2];
    DinoVec2 B = lassoPoints[lassoPoints.size() - 1];

    for (int j = 0; j < lassoPoints.size() - 4; j++) {
        DinoVec2 C = lassoPoints[j];
        DinoVec2 D = lassoPoints[j + 1];
        if (ArePointsEqual(C, D))
            continue;
        if (Dino_IntersectSegment(A, B, C, D)) {
            for (dino_Entity* entity : entities) {
                if (IsPointInLoop(entity->entityPosition, j, lassoPoints.size() - 1)) {
                    entity->LassoCatched(attachedPlayer->dinoID);
                }
            }
            lassoPoints.erase(lassoPoints.begin() + j, lassoPoints.end());
            break;
        }
    }

    std::vector<DinoVertex> vs;
    DinoColor lineColor =
        playerId == 0
            ? DinoColor_BLUE
            : playerId == 1
            ? DinoColor_RED
            : playerId == 2
            ? DinoColor_YELLOW
            : DinoColor_GREEN;

    Dino_GenVertices_Polyline(vs, lassoPoints, 5, lineColor);
    DinoVertexBuffer lassoVertexBuffer(vs.data(), vs.size(), "lasso");
    XDino_Draw(lassoVertexBuffer.Get(), XDino_TEXID_WHITE, {}, 1);
}

void dino_lasso::CutLasso(int fromIndex)
{
    lassoPoints.erase(lassoPoints.begin(), lassoPoints.begin() + fromIndex);
}

bool dino_lasso::ArePointsEqual(DinoVec2 p1, DinoVec2 p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool dino_lasso::IsPointInLoop(DinoVec2 p, int index1, int index2)
{
    DinoVec2 p2 = {0, 0};
    int intersectionCount = 0;

    for (int j = index1; j < index2; j++) {
        DinoVec2 C = lassoPoints[j];
        DinoVec2 D = lassoPoints[j + 1];
        if (ArePointsEqual(C, D))
            continue;
        if (Dino_IntersectSegment(p, p2, C, D)) {
            intersectionCount++;
        }
    }

    return intersectionCount % 2 != 0;
}