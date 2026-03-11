#pragma once

#include <dino/xdino.h>
#include <dino/dino_terrain.h>

class DinoPlayer {
    DinoVec2 m_pos = {};
    bool m_bLeft = false;
    double m_endHitAnim = 0;
    uint64_t m_texID = 0;
    bool m_bMoving = false;
    bool m_bPressedRun = false;
    int m_idxPlayer = 0;

    uint64_t GenerateVertexBuffer(double timeSinceStart);

public:
    void Init(int idxPlayer);
    static void ResolveCollision(DinoPlayer& A, DinoPlayer& B);
    void Update(double timeSinceStart, float deltaTime, DinoTerrain terrain, DinoGamepad gamepad);
    void Draw(double timeSinceStart);
    void Shut();
};