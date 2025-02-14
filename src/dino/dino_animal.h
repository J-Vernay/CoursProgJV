#pragma once

#include <dino/xdino.h>
#include <dino/dino_entity.h>

constexpr float ANIMAL_SPEED = 100.f;

class dino_animal : public DinoEntity {
    DinoVec2 m_dir = {};
    int32_t m_idxAnimal;
    bool bMirror = false;

public:
    // Met à jour la position du joueur en fonction de la manette.
    void Update(float deltaTime) override;

    // Dessine l'animal à l'écran.
    void Draw(double timeSinceStart) override;

    // Initialise le joueur.
    void InitAnimal(DinoVec2 initPos, int32_t idxAnimal);

    // Détermine si le joueur est au-dessus de l'autre.
    bool IsAbove(dino_animal& other);

protected:
    void OnTerrainBorder() override;
};