#pragma once

#include <dino/xdino.h>
#include <dino/dino_entity.h>

// Représente un animal.
class DinoAnimal : public DinoEntity {
public:
    /// Initialise l'animal avec un type au hasard.
    void InitRandom(DinoVec2 pos, double timeSinceStart);

    /// Met à jour les déplacements du animal.
    void Update(double timeSinceStart, float deltaTime);

    /// Force l'animal à prendre une nouvelle direction aléatoire.
    void SetRandomDir();

    /// Nom de la texture qui contient les animaux.
    static const std::string TEXTURE_NAME;

    /// Ajoute les vertices concernant l'animal actuel au `drawCall` donné.
    /// Gère les animations grâce au temps qui passe.
    void AddDrawCall(double timeSinceStart, float deltaTime, DinoDrawCall& drawCall) const;

private:
    double m_timeStart;
    DinoVec2 m_dir;
    int32_t m_kind;
};