#pragma once

#include "x64-windows/xdino_win64_rdr.h"

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

enum DinoState {
    Idle,
    Walking,
    Running
};


class DinoPlayer {

public:
    DinoPlayer();
    void Initialize(DinoGamepadIdx gamepadIdx, DinoVec2 pos);

    void Update(float deltaTime);
    void Draw(double timeSinceStart);

private:
    DinoGamepadIdx m_gamepadIdx;

    DinoVec2 m_pos;

    float m_movementSpeed;

    bool m_isMirror = false;
    DinoState m_state;
    int m_animationIndex;
};