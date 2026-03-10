#include <dino/dino_player.h>
#include <dino/xdino.h>

uint64_t DinoPlayer_GenerateVertexBuffer(DinoPlayer& player,
                                         double timeSinceStart, bool bMoving, bool bPressedRun)
{
    float animSpeed;
    int frameCount;
    int ubase;
    if (timeSinceStart < player.endHitAnim) {
        // ANIM HIT
        animSpeed = 8;
        frameCount = 3;
        ubase = 336;
    }
    else if (bMoving) {
        if (bPressedRun) {
            // ANIM RUN
            animSpeed = 16;
            frameCount = 6;
            ubase = 432;
        }
        else {
            // ANIM WALK
            animSpeed = 8;
            frameCount = 6;
            ubase = 96;
        }
    }
    else {
        // ANIM IDLE
        animSpeed = 8;
        frameCount = 4;
        ubase = 0;
    }

    int uAnim = ((int)(timeSinceStart * animSpeed) % frameCount) * 24 + ubase;

    std::vector<DinoVertex> vs;
    uint16_t umin, umax;
    if (player.bLeft) {
        umin = uAnim + 24;
        umax = uAnim + 0;
    }
    else {
        umin = uAnim + 0;
        umax = uAnim + 24;
    }

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin;
    vs[0].v = 0;
    vs[1].pos = {24, 0};
    vs[1].u = umax;
    vs[1].v = 0;
    vs[2].pos = {0, 24};
    vs[2].u = umin;
    vs[2].v = 24;
    vs[3].pos = {24, 0};
    vs[3].u = umax;
    vs[3].v = 0;
    vs[4].pos = {0, 24};
    vs[4].u = umin;
    vs[4].v = 24;
    vs[5].pos = {24, 24};
    vs[5].u = umax;
    vs[5].v = 24;
    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
}