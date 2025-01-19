#pragma once

#include <dino/xdino.h>

/// Représente le terrain de jeu.
class DinoTerrain {
public:
    /// Génère le terrain.
    void Init(int32_t tileCountX, int32_t tileCountY);

    /// Affiche le terrain de jeu.
    void Draw(double timeSinceStart, float deltaTime) const;

    DinoVec2 GenerateRandomSpawn() const;

private:
    // Position in pixels
    DinoVec2 m_spawnOffset;
    DinoVec2 m_spawnSize;

    DinoDrawCall m_drawCallOcean;
    DinoDrawCall m_drawCallTerrain;
    DinoDrawCall m_drawCallFlowers;
};