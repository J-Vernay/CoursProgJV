#pragma once
#include <dino/dino_terrain.h>
#include <dino/xdino.h>
#include "dino_entity.h"
#include <dino/dino_draw_utils.h>


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

struct DinoPlayer : public DinoEntity {
    bool m_bRight = false;
    double m_endHitAnim = 0;
    animState m_currentAnim = Idle;
    uint64_t m_texID;
    int m_idxPlayer = 0;
    std::vector<anim> m_anims;


    void Init(int idxPlayer);
    void Update(double timeSinceStart, float deltaTime, DinoTerrain terrain, DinoGamepad gamepad);
    void Draw(double timeSinceStart) override;
    void ReactLoop(double timeSinceStart) override;

    void Shut();

    uint64_t GenerateVertexBuffer(double timeSinceStart);
    anim Get_Current_Anim();
};