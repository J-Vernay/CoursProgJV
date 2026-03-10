#pragma once

#include <dino/xdino.h>


enum animState {
    Idle = 0,
    Walk = 1,
    Hit = 2,
    Run = 3,
};

struct anim {
    animState state;
    float Ubase;
    float NbFrames;
    float Speed;
};

struct DinoPlayer {
    DinoVec2 m_pos = {};
    bool m_bRight = false;
    double m_endHitAnim = 0;
    animState m_currentAnim = Idle;
    uint64_t m_texID;
    int m_idxPlayer = 0;

    void Init(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad);
    void Draw(double timeSinceStart, anim currentAnim);
    void Shut();

    uint64_t GenerateVertexBuffer(double timeSinceStart, anim current);
};