#pragma once
#include "xdino.h"

class DinoEntity
{
public:
    DinoVec2 GetPos() const { return pos_; }
    void SetPos(DinoVec2 pos) { pos_ = pos; }

    void ApplyTerrain(DinoVec2 a, DinoVec2 b);
    virtual void OnTerrainBorder();
    virtual void Update(float deltaTime);
    virtual void Display(double timeSinceStart);

protected:
    DinoVec2 pos_ = {};
    
};
