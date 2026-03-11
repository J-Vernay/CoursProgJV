#include "dino_draw_utils.h"
#include "dino_entity.h"
#include "xdino.h"

class Animal : public DinoEntity{
    DinoVec2 m_pos = {};
    //std::vector<DinoVertex> m_vs;
    uint64_t m_texID = 0;
    double m_endHitAnim = 0;
    DinoColor m_color = {0xFF, 0xFF, 0xFF, 0x00};
    bool m_bLeft = false;
    bool m_bMoving = false;
    float m_moveSpeed;
    int m_idxAnimal;

public:
    double m_spawnTime;
    EAnimalKind m_kind;
    EAnimalAnim m_anim;
    DinoVec2 m_targetPos = {};
    void Init();
    void Update(float deltaTime, double timeSinceStart);
    void Draw(double timeSinceStart);
    void Shut();
};