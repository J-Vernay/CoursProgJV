#pragma once

#include <dino/xdino.h>

#include <span>

/// Représente l'avatar d'un joueur dans le monde.
class DinoPlayer {
public:
    /// Crée le DinoPlayer associé à un joueur.
    void Init(DinoVec2 initPos, DinoGamepadIdx idxGamepad_, int32_t idxPlayer_);

    /// Met à jour les déplacements du joueur.
    void Update(double timeSinceStart, float deltaTime);

    /// Construit un DinoDrawCall pour l'affichage de tous les dinosaures d'un coup.
    static DinoDrawCall DrawCallDinos(std::span<DinoPlayer const> dinos, double timeSinceStart, float deltaTime);

private:
    DinoVec2 dinoPos{};
    DinoVec2 dinoMove{};
    bool bDinoLeft = false;
    bool bDinoRunning = false;
    bool bDinoDamage = false;
    double dinoDamageStart = 0;
    DinoGamepadIdx idxGamepad{};
    int32_t idxPlayer{};

    /// Ajoute les vertices concernant le joueur actuel au `drawCall` donné.
    /// Gère les animations grâce au temps qui passe.
    void _AddDrawCall(double timeSinceStart, float deltaTime, DinoDrawCall& drawCall) const;
};