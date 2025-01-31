#pragma once

#include <dino/xdino.h>
#include <dino/dino_terrain.h>

class DinoEntity {
public:
    /// Récupère la position actuelle de l'entité.
    DinoVec2 GetPos() const;

    void ClampToTerrain(DinoTerrain const& terrain);

    static void ResolveCollision(DinoEntity& entity1, DinoEntity& entity2);

protected:
    DinoVec2 m_pos = {};
};