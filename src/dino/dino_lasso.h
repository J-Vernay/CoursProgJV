#pragma once
#include "dino_actor.h"
#include "dino_draw_utils.h"

class DinoLasso {
    std::vector<DinoVec2> linePoints;
    DinoVec2 position = {};
    float deleteTimer = 0;
public:
    void update(float deltaTime, DinoVec2 newPosition);
    void draw(DinoColor color) const;
private:
    void updateAddPoints(DinoVec2 newPosition);
    void updateRemovePoints(float deltaTime);
};