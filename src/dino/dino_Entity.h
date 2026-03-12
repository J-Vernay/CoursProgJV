#pragma once

#include <dino/xdino.h>

class dino_Entity {
public :
    DinoVec2 entityPosition;
    float collisionRadius;

    void MoveIfOutOfBounds(DinoVec2 terrainTopLeft);
    virtual bool IsEntityDead();
    virtual void Update(float deltaTime);
    virtual void ReactionToBorderCross();
    virtual void DrawEntity(double timeSinceStart);
    virtual void LassoCatched(int playerId);
    static bool CompareVerticalY(dino_Entity* a, dino_Entity* b);

private:
    float GetCorrectionX(float xPos, DinoVec2 terrainTopLeft);
    float GetCorrectionY(float yPos, DinoVec2 terrainTopLeft);
};