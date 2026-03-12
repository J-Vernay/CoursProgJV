#pragma once

#include "dino_Entity.h"

#include <unordered_map>
#include <dino/xdino.h>

class dino_player : public dino_Entity {
public :
    //DinoGamepad dinoGamepad;
    static constexpr float DINO_SPEED = 150.f; // Nombre de pixels parcourus en une seconde.
    int dinoID;
    bool isStun = false;

    dino_player(DinoVec2 initialPos, float collisionRadius, uint64_t dinoTex, int dino_ID, DinoVec2 terrainTopLeft);

    struct Anim {
        std::vector<int> posU;
        int framesPerSecond;
    };

    void DinoPlayer_ReadGamePad(DinoGamepad gamepad, float deltaTime);
    void DinoPlayer_Stun();
    void Update(float deltaTime) override;
    void DrawEntity(double timeSinceStart) override;
    void LassoCatched(int playerId) override;

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

    static constexpr float STUN_TIME = 3;
    float waitedTime = 0;

    DinoVec2 m_terrainTopLeft;
};