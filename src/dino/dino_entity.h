#pragma once
#include <dino/xdino.h>
class DinoEntity {
protected:
    DinoVec2 m_pos;
public:
    DinoVec2 GetPos();
    void SetPos(DinoVec2 newPos);
};