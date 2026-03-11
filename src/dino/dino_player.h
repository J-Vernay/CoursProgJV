#pragma once

//#include <dino/dino_draw_utils.h>
#include "dino_CollisionEntity.h"

#include <unordered_map>
#include <dino/xdino.h>

class dino_player : public dino_CollisionEntity {
public :
    //DinoGamepad dinoGamepad;
    static constexpr float DINO_SPEED = 150.f; // Nombre de pixels parcourus en une seconde.

    dino_player(DinoVec2 initialPos, float collisionRadius, uint64_t dinoTex, int dino_ID, DinoVec2 terrainTopLeft);

    struct Anim {
        std::vector<int> posU;
        int framesPerSecond;
    };

    void DinoPlayer_UpdateMovement(DinoGamepad gamepad, float deltaTime,
                                   std::unordered_map<DinoGamepadIdx, dino_player> playerGamepadPair);
    void DinoPlayer_Logic(float timeSinceStart, float deltaTime);
    void DinoPlayer_Stun();

private :
    uint64_t texID_dino;

    Anim* currentAnimationPos;
    Anim idlePos;
    Anim walkPos;
    Anim hitPos;
    Anim runPos;


    bool isStatic = false;
    bool isRunning = false;
    bool isGoingLeft = false;
    bool isStun = false;

    static constexpr float STUN_TIME = 3;
    float waitedTime = 0;

    void UpdateVisual(float timeSinceStart);
    bool CanAddXValue(float xToAdd);
    bool CanAddYValue(float yToAdd);
    int dinoID;
    DinoVec2 m_terrainTopLeft;
};