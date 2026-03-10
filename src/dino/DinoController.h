#pragma once

#include <dino/xdino.h>

struct DinoControllerFields {

    // Fields
private:
    DinoVec2 dinoPos;
    double dinoCurrentSpeed;
    bool dinoGoingLeft = false;

    float dinoAnimElapsed = 0;
    float dinoDamageAnimTimer;
    int currFrame = 0;
    int dinoColor = 0;

    bool g_dinoCanTakeDamage = true;

public:
    uint64_t vbufID_dino;

    // Methods
private:
    uint64_t GenDinoVertexBuffer(DinoGamepad gamepad, float deltaTime);

public:
    void Init(int playerCount);
    // void Shut();

    void DinoMovement(DinoGamepad gamepad, float deltaTime);
    void DrawDino(DinoGamepad gamepad, float deltaTime, uint64_t texID_dino);
};