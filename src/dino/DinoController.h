#include <dino/xdino.h>

struct DinoControllerFields {
    uint64_t vbufID_dino;

    DinoVec2 dinoPos;
    double dinoCurrentSpeed;
    bool dinoGoingLeft = false;

    float dinoAnimElapsed = 0;
    float dinoDamageAnimTimer;
    int currFrame = 0;
    int dinoColor = 0;

    bool g_dinoCanTakeDamage = true;

    uint64_t DrawDino(DinoGamepad gamepad, float deltaTime, uint64_t texID_dino);
    void DinoMovement(DinoGamepad gamepad, float deltaTime);
};