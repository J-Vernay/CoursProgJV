#pragma once

//#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

class dino_player {
public :
    //DinoGamepad dinoGamepad;
    static constexpr float DINO_SPEED = 150.f; // Nombre de pixels parcourus en une seconde.

    dino_player(DinoVec2 initialPos, uint64_t dinoTex, int dino_ID, DinoVec2 terrainTopLeft);

    struct Anim {
        std::vector<int> posU;
        int framesPerSecond;
    };

    void DinoCharacter_ReadGamepad(DinoGamepad gamepad, float deltaTime);
    void DinoCharacter_Update(float timeSinceStart, float deltaTime);
    void DinoCharacter_Stun();

private :
    uint64_t texID_dino;

    Anim* currentAnimationPos;
    Anim idlePos;
    Anim walkPos;
    Anim hitPos;
    Anim runPos;

    DinoVec2 dinoPos = {};

    bool isStatic = false;
    bool isRunning = false;
    bool isGoingLeft = false;
    bool isStun = false;

    static constexpr float STUN_TIME = 3;
    float waitedTime = 0;

    void DinoCharacter_UpdateVisual(float timeSinceStart);
    bool CanAddXValue(float xToAdd);
    bool CanAddYValue(float yToAdd);
    int dinoID;
    DinoVec2 m_terrainTopLeft;
};