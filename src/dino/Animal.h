#pragma once
#include "Entity.h"

class Animal : public Entity
{
public:
    explicit Animal();
    explicit Animal(const std::string& tex_d, uint16_t id, DinoVec2 pos_d, int radius_d, uint16_t u, uint16_t v);

    void Update(float deltaTime) override;
    void Draw() override;

    bool GetActive() const;
    void SetActive(bool active);
    
    void SetDir(DinoVec2 dir);

    void Spawn();
    
private:
    uint16_t _id;
    DinoVec2 _dir;

    uint16_t _u;
    uint16_t _v;
    uint16_t _c;
    uint16_t _offset;

    float _animTimer;
    bool inverted;
    
    bool _active;
};
