#include "dino_Entity.h"

void dino_Entity::MoveIfOutOfBounds(DinoVec2 terrainTopLeft)
{
    float xPos = GetCorrectionX(entityPosition.x, terrainTopLeft);
    float yPos = GetCorrectionY(entityPosition.y, terrainTopLeft);
    if (fabs(xPos) > 0.01f || fabs(yPos) > 0.01f) {
        entityPosition.x += xPos;
        entityPosition.y += yPos;
        ReactionToBorderCross();
    }
}

bool dino_Entity::IsEntityDead()
{
    return false;
}

void dino_Entity::Update(float deltaTime)
{
}

void dino_Entity::ReactionToBorderCross()
{
}

void dino_Entity::DrawEntity(double timeSinceStart)
{
}

void dino_Entity::LassoCatched(int playerId)
{
}

float dino_Entity::GetCorrectionX(float xPos, DinoVec2 terrainTopLeft)
{
    if (xPos < terrainTopLeft.x + 8)
        return terrainTopLeft.x + 8 - xPos; //margin left 8
    if (xPos > terrainTopLeft.x + 248)
        return terrainTopLeft.x + 248 - xPos; // margin right 8
    return 0;
}

float dino_Entity::GetCorrectionY(float yPos, DinoVec2 terrainTopLeft)
{
    if (yPos < terrainTopLeft.y + 8)
        return terrainTopLeft.y + 8 - yPos; // margin top 8
    if (yPos > terrainTopLeft.y + 176)
        return terrainTopLeft.y + 176 - yPos; // margin down 16
    return 0;
}

bool dino_Entity::CompareVerticalY(dino_Entity* a, dino_Entity* b)
{
    return a->entityPosition.y < b->entityPosition.y;
}