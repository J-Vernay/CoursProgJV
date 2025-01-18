#pragma once

#include <dino/xdino.h>

/// Représente le terrain de jeu.
class DinoTerrain {
public:
    /// Génère le terrain.
    void Init(int32_t tileCountX, int32_t tileCountY);

    /// Affiche le terrain de jeu.
    void Draw(double timeSinceStart, float deltaTime);

private:
    DinoDrawCall m_drawCallOcean;
    DinoDrawCall m_drawCallTerrain;
};