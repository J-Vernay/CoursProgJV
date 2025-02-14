#pragma once
#include <dino/xdino.h>

class DinoEntity {
protected:
    DinoVec2 m_pos;

    virtual void OnTerrainBorder();

public:
    DinoVec2 GetPos();
    void SetPos(DinoVec2 newPos);

    virtual void Update(float deltaTime);
    virtual void Draw(double timeSinceStart);

    /// Ajuste la position du joueur pour rester sur le terrain.
    /// @param a Position en haut à gauche du terrain
    /// @param b Position en bas à droite du terrain
    void ApplyTerrain(DinoVec2 a, DinoVec2 b);
};