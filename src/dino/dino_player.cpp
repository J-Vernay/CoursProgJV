#include "dino_player.h"
#include "dino_terrain.h"
#include <algorithm>
#include <cmath>

DinoPlayer::DinoPlayer()
{
    pos = {400, 300};

    idleAnim = {{0, 24, 48, 72}, 8};
    walkAnim = {{96, 120, 144, 168, 192, 216}, 8};
    hitAnim = {{336, 360, 384}, 8};
    runAnim = {{432, 456, 480, 504, 528, 552}, 16};

    currentAnim = &idleAnim;
}

void DinoPlayer::Init(const DinoTerrain& terrain, int colorV)
{
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");

    // Spawn aléatoire dans le terrain
    pos = terrain.GetRandomCellCenter();

    // Couleur du dinosaure
    currentV = colorV;
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime, const DinoTerrain& terrain, const DinoGamepad& gamepad)
{
    if (timeSinceStart < hitEndTime) {
        currentAnim = &hitAnim;
        return;
    }

    isRunning = false;
    isGoingLeft = false;

    bool stickMoving = false;

    if (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0) {
        stickMoving = true;
        float speed = baseSpeed * (gamepad.btn_right ? 2.0f : 1.0f);

        pos.x += gamepad.stick_left_x * speed * deltaTime;
        pos.y += gamepad.stick_left_y * speed * deltaTime;

        isGoingLeft = gamepad.stick_left_x < 0;
        isRunning = gamepad.btn_right;
    }

    if (gamepad.btn_left) {
        hitEndTime = timeSinceStart + 3.0;
    }

    currentAnim = stickMoving ? (isRunning ? &runAnim : &walkAnim) : &idleAnim;

    pos.x = std::clamp(pos.x,
                       terrain.GetOrigin().x + spriteSize / 2,
                       terrain.GetOrigin().x + terrain.GetWidth() - spriteSize / 2);
    pos.y = std::clamp(pos.y,
                       terrain.GetOrigin().y + spriteSize / 2,
                       terrain.GetOrigin().y + terrain.GetHeight() - spriteSize / 2);
}

void DinoPlayer::Draw(double timeSinceStart) const
{
    int frameIndex = (int)(timeSinceStart * currentAnim->framesPerSecond) % currentAnim->posU.size();
    int uStart = currentAnim->posU[frameIndex];
    int uEnd = uStart + spriteSize;

    std::vector<DinoVertex> vs(6);
    vs[0].pos = {-1, -1};
    vs[1].pos = {1, -1};
    vs[2].pos = {-1, 1};
    vs[3].pos = {1, -1};
    vs[4].pos = {-1, 1};
    vs[5].pos = {1, 1};

    for (DinoVertex& v : vs)
        v.color = DinoColor_WHITE;

    if (isGoingLeft) {
        vs[0].u = uEnd;
        vs[0].v = currentV;
        vs[1].u = uStart;
        vs[1].v = currentV;
        vs[2].u = uEnd;
        vs[2].v = currentV + spriteSize;
        vs[3].u = uStart;
        vs[3].v = currentV;
        vs[4].u = uEnd;
        vs[4].v = currentV + spriteSize;
        vs[5].u = uStart;
        vs[5].v = currentV + spriteSize;
    }
    else {
        vs[0].u = uStart;
        vs[0].v = currentV;
        vs[1].u = uEnd;
        vs[1].v = currentV;
        vs[2].u = uStart;
        vs[2].v = currentV + spriteSize;
        vs[3].u = uEnd;
        vs[3].v = currentV;
        vs[4].u = uStart;
        vs[4].v = currentV + spriteSize;
        vs[5].u = uEnd;
        vs[5].v = currentV + spriteSize;
    }

    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "DinoPlayer");
    XDino_Draw(vbufID, texID_dino, pos, 12);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoPlayer::Shut()
{
    XDino_DestroyGpuTexture(texID_dino);
}

void DinoPlayer::RepulseWith(DinoPlayer& other)
{
    float radius = spriteSize / 3.0f;
    // Par 3 plutôt que 2 pour s'adapter plus fidelement au visuel des dinosaures. radius = 24/3 = 8 
    float minDistance = radius * 2.0f; // minDistance = 8 * 2 = 16
    DinoVec2 dir = {other.pos.x - pos.x, other.pos.y - pos.y};
    float dist2 = dir.x * dir.x + dir.y * dir.y;

    if (dist2 == 0.0f) {
        dir.x = 1.0f;
        dir.y = 0.0f;
        dist2 = 1.0f;
    }

    if (dist2 < minDistance * minDistance) {
        float dist = std::sqrt(dist2);
        float overlap = (minDistance - dist) / 2.0f;
        pos.x -= dir.x / dist * overlap;
        pos.y -= dir.y / dist * overlap;
        other.pos.x += dir.x / dist * overlap;
        other.pos.y += dir.y / dist * overlap;
    }
}