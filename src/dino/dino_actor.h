/// @file dino_actor.h
/// @brief Classe de base pour les acteurs du jeu.
/// N'implémente vraiment que la fonction de comparaison de hauteur.

#pragma once
#include <dino/xdino.h>

/// Acteur basique.
/// @brief Classe abstraite pour les acteurs du jeu.
/// Implémente les quelques fonctions partagées par tous les acteurs.
class DinoActor {
    friend class DinoLasso;
    DinoVec2 lastPosition;

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
    virtual void update(float deltaTime)
    {
        lastPosition = position;
    }

    /// Dessine l'acteur à l'écran.
    virtual void draw() const = 0;

    /// Détecte et résout la collision de l'acteur avec les bords du terrain de jeu.
    void handleTerrainCollision();

    /// Détecte et résout la collison entre deux acteurs.
    void handleActorCollision(DinoActor* other);

    /// Détecte et résout si l'acteur est entouré par un lasoo.
    void handleActorCircled(std::vector<DinoVec2>::iterator first, std::vector<DinoVec2>::iterator second);

    /// Compare la position verticale de deux acteurs.
    /// @param first Le premier acteur à comparer.
    /// @param second Le second acteur à comparer.
    static bool compareHeight(const DinoActor* first, const DinoActor* second);

    /// Renvoie si l'acteur est en vie ou non.
    virtual bool isAlive()
    {
        return true;
    }

    /// Renvoie si l'acteur est interactible ou non.
    virtual bool isInteractable()
    {
        return true;
    }

protected:
    /// Évenement déclenché quand la collision avec le terrain est détectée.
    virtual void onTerrainCollide()
    {
    }

    /// Évenement quand l'acteur est entouré par le lasso.
    virtual void hit()
    {
    }
};