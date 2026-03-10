#include <dino/xdino.h>

struct DinoPlayer {
    DinoVec2 pos = {};
    bool bLeft = false;
    double endHitAnim = 0;
};

uint64_t DinoPlayer_GenerateVertexBuffer(DinoPlayer& player,
    double timeSinceStart, bool bMoving, bool bPressedRun);