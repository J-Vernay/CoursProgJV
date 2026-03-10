#include "xdino.h"

class Animal {
    DinoVec2 m_pos = {};
    bool m_bLeft = false;
    double m_endHitAnim = 0;
    uint64_t m_texID = 0;
    bool m_bMoving = false;
    uint64_t GenerateVertexBuffer();

public:
    void Init();
    void Update(double timeSinceStart, float deltaTime, DinoVec2 direction);
    void Draw(int idxAnimal);
    void Shut();
};