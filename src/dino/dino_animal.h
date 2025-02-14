#pragma once
#include <dino/xdino.h>
#include <dino/dino_entity.h>

class dino_animal : public dino_entity {
private:
    DinoVec2 direction = {};
    int32_t idxAnimal;

    bool isMirror = false;
    bool isIdle = false;
    bool isWalking = false;

    void OnTerrainBorder() override;

public:
    void Init(DinoVec2 initPos, int32_t indexAnimal);

    /// Met à jour la position de l'animal et sa logique d'animation.
    void Update(float deltaTime) override;

    /// Affiche l'animal à l'écran.
    void Draw(double timeSinceStart) override;

    /// Compare la position de deux animaux pour savoir lequel afficher devant l'autre.
    bool IsAbove(dino_animal& other);
};