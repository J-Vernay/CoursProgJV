#pragma once

#include <dino/xdino.h>


/// Représente le lasso d'un dinosaure.
class DinoLasso {
public:
    /// Initialise le lasso d'un dinosaure avec une couleur donnée.
    void Init(DinoColor color);

    /// Met à jour le lasso en prenant en compte la nouvelle position du dinosaure.
    void Update(DinoVec2 newPoint);

    /// Affiche le lasso à l'écran.
    void Draw() const;

    /// Réagit si une boucle est formée par le dernier appel à Update(), et réagit.
    void ApplyCollisionSelf();

    /// Réagit si le dernier appel à Update() cause une collision avec l'autre lasso.
    void ApplyCollisionTo(DinoLasso& other) const;

private:
    std::vector<DinoVec2> m_points;
    DinoColor m_color;
};