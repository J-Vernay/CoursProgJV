#pragma once

#include <dino/xdino.h>

/// Représente le terrain de jeu.
class DinoTerrain {
public:
    /// Génère le terrain.
    void Init(int32_t tileCountX, int32_t tileCountY);

    /// Affiche le terrain de jeu.
    void Draw(double timeSinceStart, float deltaTime) const;

    /// Accesseurs pour l'extérieur, quand on a besoin de positionner des éléments sur le terrain.
    DinoVec2 GetSpawnOffset() const
    {
        return m_spawnOffset;
    }

    DinoVec2 GetSpawnSize() const
    {
        return m_spawnSize;
    }

private:
    // Position in pixels
    DinoVec2 m_spawnOffset;
    DinoVec2 m_spawnSize;

    DinoDrawCall m_drawCallOcean;
    DinoDrawCall m_drawCallTerrain;
    DinoDrawCall m_drawCallFlowers;
};