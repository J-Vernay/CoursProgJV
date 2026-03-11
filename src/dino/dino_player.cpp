#include "dino_player.h"

dino_player::dino_player(DinoVec2 initialPos, float collisionRadius, uint64_t dinoTex, int dino_ID,
                         DinoVec2 terrainTopLeft)
{
    entityPosition = initialPos;
    this->collisionRadius = collisionRadius;

    texID_dino = dinoTex;
    dinoID = dino_ID;

    idlePos = {{0, 24, 48, 72}, 8};
    walkPos = {{96, 120, 144, 168, 192, 216}, 8};
    hitPos = {{336, 360, 384}, 8};
    runPos = {{432, 456, 480, 504, 528, 552}, 16};

    currentAnimationPos = &idlePos;
    m_terrainTopLeft = terrainTopLeft;
}

void dino_player::DinoPlayer_Logic(float timeSinceStart, float deltaTime)
{
    if (isStun) {
        waitedTime += deltaTime;
        if (waitedTime >= STUN_TIME) {
            isStun = false;
        }
    }

    UpdateVisual(timeSinceStart);
}

void dino_player::UpdateVisual(float timeSinceStart)
{
    if (isStun)
        currentAnimationPos = &hitPos;
    else if (isStatic)
        currentAnimationPos = &idlePos;
    else {
        currentAnimationPos = isRunning ? &runPos : &walkPos;
    }

    int index = isGoingLeft
                    ? (int)(timeSinceStart * currentAnimationPos->framesPerSecond) % (
                          currentAnimationPos->posU.size() - 1) + 1
                    : (int)(timeSinceStart * currentAnimationPos->framesPerSecond) % (
                          currentAnimationPos->posU.size() - 1);

    int u1 = (*currentAnimationPos).posU[index];
    int u2 = isGoingLeft
                 ? (*currentAnimationPos).posU[(index - 1) % currentAnimationPos->posU.size()]
                 : (*currentAnimationPos).posU[(index + 1) % currentAnimationPos->posU.size()];

    int baseV = dinoID * 24;

    std::vector<DinoVertex> vs;
    {
        vs.resize(6);
        vs[0].pos = {-1, -1};
        vs[0].color = DinoColor_WHITE;
        vs[1].pos = {1, -1};
        vs[1].color = DinoColor_WHITE;
        vs[2].pos = {-1, 1};
        vs[2].color = DinoColor_WHITE;
        vs[3].pos = {1, -1};
        vs[3].color = DinoColor_WHITE;
        vs[4].pos = {-1, 1};
        vs[4].color = DinoColor_WHITE;
        vs[5].pos = {1, 1};
        vs[5].color = DinoColor_WHITE;
        vs[0].u = u1;
        vs[0].v = baseV;
        vs[1].u = u2;
        vs[1].v = baseV;
        vs[2].u = u1;
        vs[2].v = baseV + 24;
        vs[3].u = u2;
        vs[3].v = baseV;
        vs[4].u = u1;
        vs[4].v = baseV + 24;
        vs[5].u = u2;
        vs[5].v = baseV + 24;
    }

    uint64_t vbufID_dino1 = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
    XDino_Draw(vbufID_dino1, texID_dino, entityPosition, 12);
    XDino_DestroyVertexBuffer(vbufID_dino1);
}

bool dino_player::CanAddXValue(float xToAdd)
{
    return !(entityPosition.x + xToAdd < m_terrainTopLeft.x + 8 || //margin left 8
             entityPosition.x + xToAdd > m_terrainTopLeft.x + 248); // margin right 8
}

bool dino_player::CanAddYValue(float yToAdd)
{
    return !(entityPosition.y + yToAdd < m_terrainTopLeft.y + 8 || // margin top 8
             entityPosition.y + yToAdd > m_terrainTopLeft.y + 176); // margin down 16
}


void dino_player::DinoPlayer_UpdateMovement(DinoGamepad gamepad, float deltaTime,
                                            std::unordered_map<DinoGamepadIdx, dino_player> playerGamepadPair)
{
    if (!isStun) {
        float xToAdd = isRunning
                           ? gamepad.stick_left_x * DINO_SPEED * deltaTime * 2
                           : gamepad.stick_left_x * DINO_SPEED * deltaTime;
        float yToAdd = isRunning
                           ? gamepad.stick_left_y * DINO_SPEED * deltaTime * 2
                           : gamepad.stick_left_y * DINO_SPEED * deltaTime;

        entityPosition.x += CanAddXValue(xToAdd) ? xToAdd : 0;
        entityPosition.y += CanAddYValue(yToAdd) ? yToAdd : 0;
    }

    //checking if dino is going left or right
    if (isGoingLeft && gamepad.stick_left_x > 0)
        isGoingLeft = false;
    else if (!isGoingLeft && gamepad.stick_left_x < 0)
        isGoingLeft = true;

    //checking if dino is running
    isRunning = gamepad.btn_right;
    isStatic = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;

    if (gamepad.btn_left && !isStun)
        DinoPlayer_Stun();
}

void dino_player::DinoPlayer_Stun()
{
    isStun = true;
    waitedTime = 0;
}