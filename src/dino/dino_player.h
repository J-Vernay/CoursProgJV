#pragma once

#include <dino/xdino.h>

constexpr float CIRCLE_SPEED = 150.f;

class dino_player {
    DinoVec2 pos = {};
    bool bMirror = false;
    int idxPlayer = 0;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;

    DinoGamepadIdx m_idxGamepad;

public:
    // Met à jour la position du joueur en fonction de la manette.
    void UpdatePlayer(float deltaTime);

    // Dessine le joueur à l'écran.
    void DrawDino(double timeSinceStart);

    // Initialise le joueur.
    void InitDino(DinoVec2 position, int index, DinoGamepadIdx idxGamepad);

    // Détermine si le joueur est au-dessus de l'autre.
    bool IsAbove(dino_player& other);
};