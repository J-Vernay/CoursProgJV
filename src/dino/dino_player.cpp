#include "dino_player.h"
#include "dino_terrain.h"
#include <algorithm>

DinoPlayer::DinoPlayer()
{
    pos = {400, 300};

    idleAnim = {{0, 24, 48, 72}, 8};
    walkAnim = {{96, 120, 144, 168, 192, 216}, 8};
    hitAnim = {{336, 360, 384}, 8};
    runAnim = {{432, 456, 480, 504, 528, 552}, 16};

    currentAnim = &idleAnim;
}

void DinoPlayer::Init(const DinoTerrain& terrain)
{
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");

    // Spawn aléatoire dans le terrain
    pos = terrain.GetRandomCellCenter();
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime, const DinoTerrain& terrain)
{
    bool stickMoving = false;

    if (timeSinceStart < hitEndTime) {
        currentAnim = &hitAnim;
        return;
    }

    isRunning = false;
    isGoingLeft = false;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        if (!XDino_GetGamepad(gamepadIdx, gamepad))
            continue;

        if (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0) {
            stickMoving = true;
            float speed = baseSpeed * (gamepad.btn_right ? 2.f : 1.f);

            pos.x += gamepad.stick_left_x * speed * deltaTime;
            pos.y += gamepad.stick_left_y * speed * deltaTime;

            pos.x = std::clamp(pos.x,
                               terrain.GetOrigin().x + terrain.GetTileSize(),
                               terrain.GetOrigin().x + terrain.GetWidth() - terrain.GetTileSize());
            pos.y = std::clamp(pos.y,
                               terrain.GetOrigin().y + terrain.GetTileSize(),
                               terrain.GetOrigin().y + terrain.GetHeight() - terrain.GetTileSize());

            isGoingLeft = gamepad.stick_left_x < 0;
            isRunning = gamepad.btn_right;
        }

        if (gamepad.btn_left) {
            hitEndTime = timeSinceStart + 3.0;
        }
    }

    if (!stickMoving)
        currentAnim = &idleAnim;
    else
        currentAnim = isRunning ? &runAnim : &walkAnim;
}

void DinoPlayer::Draw(double timeSinceStart) const
{
    int frameIndex = static_cast<int>(timeSinceStart * currentAnim->framesPerSecond)
                     % currentAnim->posU.size();
    int uStart = currentAnim->posU[frameIndex];
    int uEnd = uStart + 24;

    std::vector<DinoVertex> vs(6);
    vs[0].pos = {-1, -1};
    vs[1].pos = {1, -1};
    vs[2].pos = {-1, 1};
    vs[3].pos = {1, -1};
    vs[4].pos = {-1, 1};
    vs[5].pos = {1, 1};

    for (int i = 0; i < 6; ++i)
        vs[i].color = DinoColor_WHITE;

    if (isGoingLeft) {
        vs[0].u = uEnd;
        vs[0].v = currentV;
        vs[1].u = uStart;
        vs[1].v = currentV;
        vs[2].u = uEnd;
        vs[2].v = currentV + 24;
        vs[3].u = uStart;
        vs[3].v = currentV;
        vs[4].u = uEnd;
        vs[4].v = currentV + 24;
        vs[5].u = uStart;
        vs[5].v = currentV + 24;
    }
    else {
        vs[0].u = uStart;
        vs[0].v = currentV;
        vs[1].u = uEnd;
        vs[1].v = currentV;
        vs[2].u = uStart;
        vs[2].v = currentV + 24;
        vs[3].u = uEnd;
        vs[3].v = currentV;
        vs[4].u = uStart;
        vs[4].v = currentV + 24;
        vs[5].u = uEnd;
        vs[5].v = currentV + 24;
    }

    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "DinoPlayer");
    XDino_Draw(vbufID, texID_dino, pos, 12);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoPlayer::Shut()
{
    XDino_DestroyGpuTexture(texID_dino);
}