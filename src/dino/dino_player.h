#pragma once

#include <dino/xdino.h>

class dino_player {
    DinoVec2 playerPos = {};
    bool isMirror = false;
    bool isIdle;
    bool isWalking;
    bool isRunning;
    int indexPlayer = 0;

public:
    /// Configure le joueur pour repondre à une manette en particulier
    void Init(DinoVec2 posInit, int32_t index, DinoGamepadIdx idxGamepad);

    /// Met à jour la position du joueur et sa logique d'animation
    void UpdatePlayer(float deltaTime);

    /// Affiche le joueur à l'écran
    void DrawPlayer(double timeSinceStart);

    /// Compare la position de deux joueurs pour savoir leqeul afficher devant l'autre
    bool IsAbove(dino_player& other);
};