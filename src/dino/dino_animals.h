#include "xdino.h"

class Animal {
    DinoVec2 m_pos = {};

    uint64_t GenerateVertexBuffer(double timeSinceStart);
    uint64_t m_texID = 0;
    double m_endHitAnim = 0;
    bool m_bLeft = false;
    bool m_bMoving = false;
    float m_moveSpeed;
    int m_idxAnimal;

public:
    DinoVec2 m_targetPos = {};
    void Init();
    void Update(float deltaTime);
    void Draw(double timeSinceStart);
    void Shut();
};