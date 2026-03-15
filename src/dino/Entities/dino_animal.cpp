#include <dino/Entities/dino_animal.h>

dino_animal::dino_animal(DinoVec2 terrainTopLeft, float collisionRadius)
{
    this->collisionRadius = collisionRadius;

    m_terrainTopLeft = terrainTopLeft;
    animalType = (EAnimalKind)XDino_RandomInt32(0, 7);

    DinoAnimal_GetRandomPos();
    DinoAnimal_GetRandomDirection();

    timeAlive = 0;
}

void dino_animal::Update(float deltaTime)
{
    constexpr float SPEED = 30;
    timeAlive += deltaTime;

    entityPosition.x += animalMovingDirection.x * deltaTime * SPEED;
    entityPosition.y += animalMovingDirection.y * deltaTime * SPEED;
}

void dino_animal::DinoAnimal_InstantDespawn(std::vector<dino_Entity>& entities, int index)
{
    entities.erase(entities.begin() + index);
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

bool dino_animal::IsEntityDead()
{
    return wasCatched;
}

void dino_animal::ReactionToBorderCross()
{
    DinoAnimal_GetRandomDirection();
}

void dino_animal::DrawEntity(double timeSinceStart)
{
    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, animalType, animalAnimDirection, timeSinceStart);

    for (DinoVertex& v : vs)
        v.color.a = std::min(timeAlive / apparitionTime, (float)1) * 255;

    DinoVertexBuffer vbufID_animal(vs.data(), vs.size(), "animal");

    XDino_Draw(vbufID_animal.Get(), textIdAnimal, {entityPosition.x - 16, entityPosition.y - 16}, 1);
}

void dino_animal::LassoCatched(int playerId)
{
    wasCatched = true;
}