/// @file dino_actor.h
/// @brief Classe de base pour les acteurs du jeu.
/// N'implémente vraiment que la fonction de comparaison de hauteur.

#pragma once
#include <dino/xdino.h>

/// Acteur basique.
/// @brief Classe abstraite pour les acteurs du jeu.
/// N'implémente vraiment que la fonction de comparaison de hauteur.
class DinoActor {
    friend class DinoLasso;

protected:
    DinoVec2 position; ///< Position de l'acteur à l'écran.

public:
    /// Le constructeur de l'acteur.
    /// @param position La position où placer l'acteur à son apparition.
    explicit DinoActor(DinoVec2 position) : position(position)
    {
    }

    virtual ~DinoActor() = default;

    /// Execute la logique de l'acteur pour une frame.
    /// @param deltaTime Le temps écoulé depuis la dernière frame (en ms).
    virtual void update(float deltaTime) = 0;
    /// Déssine l'acteur à l'écran.
    virtual void draw() const = 0;

    /// Compare la position verticale de deux acteurs.
    /// @param first Le premier acteur à comparer.
    /// @param second Le second acteur à comparer.
    static bool compareHeight(const DinoActor* first, const DinoActor* second);
    
    void handleTerrainCollision();

protected:
    virtual void onTerrainCollide(){}
};