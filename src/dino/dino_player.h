#pragma once

#include <dino/xdino.h>
#include <dino/dino_entity.h>
#include <dino/dino_draw_utils.h>

constexpr float CIRCLE_SPEED = 150.f;

class dino_player : public DinoEntity {
    bool bMirror = false;
    int idxPlayer = 0;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;

    DinoGamepadIdx m_idxGamepad;

    std::vector<DinoVec2> lassoPoints;
    DinoColor m_color;

public:
    // Met à jour la position du joueur en fonction de la manette.
    void Update(float deltaTime) override;

    // Dessine le joueur à l'écran.
    void Draw(double timeSinceStart) override;

    // Initialise le joueur.
    void InitDino(DinoVec2 position, int index, DinoGamepadIdx idxGamepad, DinoColor color);

    // Détermine si le joueur est au-dessus de l'autre.
    bool IsAbove(dino_player& other);

    void DrawLasso();
};