#include "dino_animal.h"

void dino_animal::DinoAnimal_Spawn(DinoVec2 terrainTopLeft, float collisionRadius)
{
    this->collisionRadius = collisionRadius;

    m_terrainTopLeft = terrainTopLeft;
    animalType = (EAnimalKind)XDino_RandomInt32(0, 7);

    DinoAnimal_GetRandomPos();
    DinoAnimal_GetRandomDirection();

    //debug
    timeAlive = 0;
}

void dino_animal::DinoAnimal_Update(double timeSinceStart, float deltaTime)
{
    constexpr float SPEED = 30;
    timeAlive += deltaTime;

    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, animalType, animalAnimDirection, timeSinceStart);

    for (DinoVertex& v : vs)
        v.color.a = std::min(timeAlive / apparitionTime, (float)1) * 255;

    uint64_t vbufID_animal = XDino_CreateVertexBuffer(vs.data(), vs.size(), "animal");

    float xToAdd = animalMovingDirection.x * deltaTime * SPEED;
    float yToAdd = animalMovingDirection.y * deltaTime * SPEED;
    if (!CanAddXValue(xToAdd) || !CanAddYValue(yToAdd)) {
        DinoAnimal_GetRandomDirection();
    }
    else {
        entityPosition.x += xToAdd;
        entityPosition.y += yToAdd;
    }
    XDino_Draw(vbufID_animal, textIdAnimal, {entityPosition.x - 16, entityPosition.y - 16}, 1);
    XDino_DestroyVertexBuffer(vbufID_animal);
}

void dino_animal::DinoAnimal_InstantDespawn(std::vector<dino_animal>& animals, int index)
{
    animals.erase(animals.begin() + index);
}

void dino_animal::DinoAnimal_GetRandomPos()
{
    //values do account for sprite marging
    float Dx = m_terrainTopLeft.x + XDino_RandomInt32(0, 240);
    float Dy = m_terrainTopLeft.y + XDino_RandomInt32(0, 168);

    entityPosition = DinoVec2(Dx, Dy);
}

void dino_animal::DinoAnimal_GetRandomDirection()
{
    animalMovingDirection = XDino_RandomUnitVec2();
    if (abs(animalMovingDirection.x) > abs(animalMovingDirection.y)) {
        if (animalMovingDirection.x > 0)
            animalAnimDirection = EAnimalAnim::Right;
        else
            animalAnimDirection = EAnimalAnim::Left;
    }
    else {
        if (animalMovingDirection.y > 0)
            animalAnimDirection = EAnimalAnim::Down;
        else
            animalAnimDirection = EAnimalAnim::Up;
    }
}

uint64_t dino_animal::textIdAnimal;

void dino_animal::DinoAnimal_InitStatic()
{
    textIdAnimal = XDino_CreateGpuTexture("animals.png");
}

void dino_animal::DinoAnimal_ShutStatic()
{
    XDino_DestroyGpuTexture(textIdAnimal);
}

bool dino_animal::CanAddXValue(float xToAdd)
{
    return !(entityPosition.x + xToAdd < m_terrainTopLeft.x + 0 || //margin left 0
             entityPosition.x + xToAdd > m_terrainTopLeft.x + 240); // margin right 16
}

bool dino_animal::CanAddYValue(float yToAdd)
{
    return !(entityPosition.y + yToAdd < m_terrainTopLeft.y + 0 || // margin top 0
             entityPosition.y + yToAdd > m_terrainTopLeft.y + 168); // margin down 24
}