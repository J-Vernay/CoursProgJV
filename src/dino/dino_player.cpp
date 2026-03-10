#include "dino_player.h"
#include <cmath>

// Définition statique des données d'animation
static AnimRange s_animations[COUNT] = {
    {0, 4, 4}, // IDLE
    {5, 6, 8}, // WALK
    {17, 7, 10}, // RUN
    {14, 3, 4} // DAMAGE
};

Player g_player; // L'instance réelle

void Player::Init(DinoVec2 startPos)
{
    pos = startPos;
    SetState(IDLE);
}

void Player::SetState(DinoState newState)
{
    if (state == newState)
        return;
    state = newState;

    AnimRange& anim = s_animations[state];
    startFrame = anim.start;
    numberOfFrames = anim.count;
    animationSpeed = anim.speed;
    currentFrame = startFrame; // Reset l'anim
}

void Player::Update(float deltaTime)
{
    // Mise à jour de la frame d'animation
    currentFrame += deltaTime * animationSpeed;
    if (currentFrame >= startFrame + numberOfFrames) {
        currentFrame = startFrame;
    }
    currentImageIndex = static_cast<int>(currentFrame);
}

void Player::Draw(uint64_t texID_dino)
{
    std::vector<DinoVertex> vs;
    uint16_t umin, umax;

    if (lookLeft) {
        umin = 24 + (24 * currentImageIndex);
        umax = 0 + (24 * currentImageIndex);
    }
    else {
        umin = 0 + (24 * currentImageIndex);
        umax = 24 + (24 * currentImageIndex);
    }

    vs.resize(6);
    // Simplification : utilise les coordonnées locales pour vs[i].pos
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

    uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "DinoPlayer");
    XDino_Draw(vbufID, texID_dino, pos, 1);
    XDino_DestroyVertexBuffer(vbufID);
}