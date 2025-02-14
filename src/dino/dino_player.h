#pragma once

#include <dino/xdino.h>

#include <dino/dino_entity.h>

/// Représente l'état d'un animal.
class DinoPlayer : public DinoEntity {
    bool goingRight = false;
    bool idle;
    bool walking;
    bool running;
    int playerIndex = 0;

public:
    /// Configure le joueur
    void Init(int idx, DinoVec2 posInit);
    /// Met a jour la position du joueur
    void Update(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize) override;
    /// Affiche le joueur à l'ecran
    void Draw(double timeSinceStart) override;
    /// Compare la position de deux joueurs pour determiner si le premier est au dessus de l'autre
    bool IsAbove(DinoPlayer& player);

};