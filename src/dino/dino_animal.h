#pragma once

#include <dino/xdino.h>

#include <dino/dino_entity.h>

/// Représente l'état d'un animal.
class DinoAnimal : public DinoEntity {
    bool goingRight = false;
    int animalType;
    DinoVec2 dir;
    float speed = 0;

    void OnTerrainBorder() override;

public:
    /// Configure le joueur
    void SpawnAnimal(int type, DinoVec2 renderSize, DinoVec2 terrainSize);
    /// Met a jour la position du joueur
    void Update(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize) override;
    /// Affiche le joueur à l'ecran
    void Draw(double timeSinceStart) override;
    /// Compare la position de deux joueurs pour determiner si le premier est au dessus de l'autre
    bool IsAbove(DinoAnimal& animal);


};