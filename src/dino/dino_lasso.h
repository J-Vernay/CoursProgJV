/// @file dino_lasso.h
/// @brief Un lasso, suite de points qui suivent la position d'un `DinoActor`
/// et peuvent déclencher des évènements quand des boucles sont formées.

#pragma once
#include <functional>
#include <dino/dino_actor.h>

class DinoPlayer;

/// Un lasso suivant un `DinoActor`.
/// @brief Un lasso, suite de points qui suivent la position d'un `DinoActor`
/// et peuvent déclencher des évènements quand des boucles sont formées.
class DinoLasso {
    using intersection_callback = std::function<void (std::vector<DinoVec2>::iterator, std::vector<DinoVec2>::iterator)>;
    
    std::vector<DinoVec2> linePoints;
    DinoVec2 position = {};
    float deleteTimer = 0;

public:
    /// Execute la logique du lasso pour une frame.
    /// @param deltaTime Le temps écoulé depuis la dernière frame (en ms).
    /// @param actor L'acteur que le lasso suit.
    void update(float deltaTime, DinoActor* actor);

    /// Dessine le lasso à l'écran.
    /// @param color Couleur avec laquelle dessiner le lasso.
    void draw(DinoColor color) const;

    /// Détecte et résout la collision entre le lasso et un acteur.
    void handleActorCollision(DinoActor* actor);
    
    /// Détecte et résout les boucles dans le lasso.
    void handleSelfIntersection(const intersection_callback& callback);

private:
    void handleAddingPoints(DinoVec2 newPosition);
    void handleRemovingPoints(float deltaTime);
};