#pragma once
#include <dino/dino_actor.h>

class DinoPlayer;

class DinoLasso {
    std::vector<DinoVec2> linePoints;
    DinoVec2 position = {};
    float deleteTimer = 0;
public:
    void update(float deltaTime, DinoVec2 newPosition);
    void draw(DinoColor color) const;
    void handlePlayerCollision(DinoPlayer* player);
private:
    void handleAddingPoints(DinoVec2 newPosition);
    void handleRemovingPoints(float deltaTime);
    void handleSelfIntersection();
};