#include <dino/Entities/dino_animal.h>
#include <dino/GameManager/DinoGameState.h>

dino_animal::dino_animal(DinoGameState& dino_game_state, float collisionRadius)
{
    this->collisionRadius = collisionRadius;

    m_dinoGameState = &dino_game_state;
    animalType = (EAnimalKind)XDino_RandomInt32(0, 7);

    DinoAnimal_GetRandomPos();
    DinoAnimal_GetRandomDirection();

    timeAlive = 0;
}

void dino_animal::Update(float deltaTime)
{
    constexpr float SPEED = 10;
    timeAlive += deltaTime;

    if (!wasCatched) {
        entityPosition.x += animalMovingDirection.x * deltaTime * SPEED;
        entityPosition.y += animalMovingDirection.y * deltaTime * SPEED;
    }
}

void dino_animal::DinoAnimal_GetRandomPos()
{
    //values do account for sprite marging
    float Dx =
        m_dinoGameState->g_terrainTopLeft.x + XDino_RandomInt32(0, 240);
    float Dy = m_dinoGameState->g_terrainTopLeft.y + XDino_RandomInt32(0, 168);

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
    return (wasCatched && timeAlive > despawnTime);
}

void dino_animal::ReactionToBorderCross()
{
    DinoAnimal_GetRandomDirection();
}

void dino_animal::DrawEntity(double timeSinceStart)
{
    std::vector<DinoVertex> vs;
    Dino_GenVertices_Animal(vs, animalType, animalAnimDirection, wasCatched ? 0 : timeSinceStart);

    for (DinoVertex& v : vs)
        v.color.a = std::min(timeAlive / apparitionTime, (float)1) * 255;

    DinoVertexBuffer vbufID_animal(vs.data(), vs.size(), "animal");

    XDino_Draw(vbufID_animal.Get(), textIdAnimal, {entityPosition.x - 16, entityPosition.y - 16}, 1);

    if (wasCatched) {
        popUPosition = popUPosition + DinoVec2{0, -0.5f};
        std::string text = std::format("+{0:02}", givenPoints);
        DinoColor textColor =
            catchingPlayerId == 0
                ? DinoColor_BLUE
                : catchingPlayerId == 1
                ? DinoColor_RED
                : catchingPlayerId == 2
                ? DinoColor_YELLOW
                : DinoColor_GREEN;

        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, textColor, DinoColor{255, 255, 255, 50});
        DinoVec2 position = popUPosition + entityPosition + DinoVec2{-textSize.x / 2, 0};
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "playerScore");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, position, 1);
    }
}

void dino_animal::LassoCatched(int playerId)
{
    if (wasCatched)
        return;

    timeAlive = 0;
    catchingPlayerId = playerId;
    wasCatched = true;
    givenPoints = m_dinoGameState->g_scoreManager.AddScore(playerId, animalType);
    popUPosition = {0, -20};
}