#pragma once

#include <dino/xdino.h>

class DinoEntity {
protected:
    DinoVec2 playerPos;

    virtual void OnTerrainBorder();

public:
    bool isAbove(DinoEntity& other);
    void Set(DinoVec2& newPos);
    DinoVec2 GetPos();

    virtual void Update(float deltaTime);
    virtual void Draw(double timeSinceStart);

    /// Ajuste la position du joueur pour rester sur le terrain.
    /// @param a Position en haut a gauche du terrain
    /// @param b Position en bas a droite du terrain
    void ApplyTerrain(DinoVec2 a, DinoVec2 b);
};