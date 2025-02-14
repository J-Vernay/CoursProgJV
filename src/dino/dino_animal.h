#pragma once

#include <dino/xdino.h>

class DinoAnimal {
    DinoVec2 pos = {};
    bool goingRight = false;
    int animalType;
    DinoVec2 dir;
    float speed = 0;

public:
    /// Configure le joueur
    void SpawnAnimal(int type, DinoVec2 renderSize, DinoVec2 terrainSize);
    /// Met a jour la position du joueur
    void UpdateAnimal(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize);
    /// Affiche le joueur à l'ecran
    void DrawAnimal(double timeSinceStart);
    /// Compare la position de deux joueurs pour determiner si le premier est au dessus de l'autre
    bool IsAbove(DinoAnimal& animal);
};