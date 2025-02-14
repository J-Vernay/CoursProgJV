#pragma once

#include <dino/xdino.h>

class DinoPlayer {
    DinoVec2 pos = {};
    bool goingRight = false;
    bool idle;
    bool walking;
    bool running;
    int playerIndex = 0;

public:
    /// Configure le joueur
    void Init(int idx, DinoVec2 posInit);
    /// Met a jour la position du joueur
    void UpdatePlayer(float deltaTime, DinoVec2 windowSize, DinoVec2 terrainSize);
    /// Affiche le joueur à l'ecran
    void DrawPlayer(double timeSinceStart);
    /// Compare la position de deux joueurs pour determiner si le premier est au dessus de l'autre
    bool IsAbove(DinoPlayer& player);
};