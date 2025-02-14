#pragma once

#include <dino//dino_draw_utils.h>
#include <dino/dino_entity.h>

class dino_animal : public dino_entity {
private:
    DinoVec2 direction = {};
    int32_t idxAnimal;

    bool isMirror = false;
    bool isIdle = false;
    bool isWalking = false;

public:
    void Init(DinoVec2 initPos, int32_t indexAnimal);
    void UpdateAnimal(float deltaTime);
    void DrawAnimal(double timeSinceStart);
    bool IsAbove(dino_animal& other);

    // Ajuste la position de l'animal pour rester sur le terrain
    // param a correspond à la position en haut à gauche du terrain et param b la position en bas à droite.
    void ApplyTerain(DinoVec2 a, DinoVec2 b);
};