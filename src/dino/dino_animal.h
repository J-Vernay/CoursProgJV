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
    void SpawnAnimal(int type);
    /// Met a jour la position du joueur
    void UpdateAnimal(float deltaTime);
    /// Affiche le joueur à l'ecran
    void DrawAnimal(double timeSinceStart);
    /// Compare la position de deux joueurs pour determiner si le premier est au dessus de l'autre
    bool IsAbove(DinoAnimal& animal);
};