#include "xdino.h"

class Animal {
    DinoVec2 m_pos = {};
    
    uint64_t GenerateVertexBuffer();
    uint64_t m_texID = 0;
    double m_endHitAnim = 0;
    bool m_bLeft = false;
    bool m_bMoving = false;
    int m_idxAnimal; 

public:
    DinoVec2 m_targetPos = {};
    void Init();
    void Update(float deltaTime);
    void Draw();
    void Shut();
};