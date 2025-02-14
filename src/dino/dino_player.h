#pragma once

#include <dino/xdino.h>
#include <dino/dino_entity.h>

/// Représente l'état d'un joueur.
class dino_player : public dino_entity {
private:
    bool isMirror = false;
    bool isIdle;
    bool isWalking;
    bool isRunning;

    int indexPlayer = 0;
    DinoGamepadIdx indexGamepad;

    std::vector<DinoVec2> lasso;
    DinoColor lassoColor;

public:
    /// Configure le joueur pour repondre à une manette en particulier
    void Init(DinoVec2 posInit, int32_t index, DinoGamepadIdx idxGamepad, DinoColor colorLasso);

    /// Met à jour la position du joueur et sa logique d'animation.
    void Update(float deltaTime) override;

    /// Ne dessine que le lasso du joueur.
    void DrawLasso();

    /// Affiche le joueur à l'écran.
    void Draw(double timeSinceStart) override;

    /// Compare la position de deux joueurs pour savoir leqeul afficher devant l'autre
    bool IsAbove(dino_player& other);
};