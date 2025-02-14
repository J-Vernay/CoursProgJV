#pragma once

#include <dino/xdino.h>

class DinoEntity {
public:
    DinoVec2 GetPosition();

    void SetPosition(DinoVec2 newPos);

    void ApplyTerrain(DinoVec2 a, DinoVec2 b);

    virtual void Update(float deltaTime);

    virtual void Draw(double timeSinceStart);

protected:
    DinoVec2 pos = {};

    virtual void OnTerrainBorder();
};