#pragma once
#include <dino/xdino.h>

class dino_entity {
protected:
    DinoVec2 pos;

    virtual void OnTerrainBorder();

public:
    DinoVec2 GetPos();
    void SetPos(DinoVec2 newPos);

    /// Met à jour la position de l'entité et sa logique d'animation
    virtual void Update(float deltaTime);

    /// Affiche l'entité à l'écran
    virtual void Draw(double timeSinceStart);

    // Ajuste la position de l'entité pour rester sur le terrain
    // param a correspond à la position en haut à gauche du terrain et param b la position en bas à droite.
    void ApplyTerain(DinoVec2 a, DinoVec2 b);
};