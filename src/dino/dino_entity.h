#pragma once
#include <dino/xdino.h>

class dino_entity {
protected:
    DinoVec2 pos;

    virtual void OnTerrainBorder();

public:
    DinoVec2 GetPos();
    void SetPos(DinoVec2 newPos);

    // Ajuste la position de l'entité pour rester sur le terrain
    // param a correspond à la position en haut à gauche du terrain et param b la position en bas à droite.
    void ApplyTerain(DinoVec2 a, DinoVec2 b);
};