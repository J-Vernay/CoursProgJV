

#include "xdino.h"

struct DinoPlayer {
    DinoVec2 pos;
    bool bMirror;
    bool bIdle;
    bool bWalking;
    bool bRunning;

    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);
};