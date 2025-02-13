
#include <dino/xdino.h>

/// Représente l'état d'un joueur.
class DinoAnimal {
private:
    DinoVec2 m_pos = {};
    DinoVec2 m_dir = {};
    int32_t m_idxAnimal;
    bool m_bMirror = false;
    
public:
    /// Initialise un animal. `idxAnimal` doit être entre 0 et 7.
    void Init(DinoVec2 initPos, int32_t idxAnimal);

    /// Met à jour la position de l'animal et sa logique d'animation.
    void UpdateAnimal(float deltaTime);

    /// Affiche l'animal à l'écran.
    void DrawAnimal(double timeSinceStart);
    
    /// Compare la position de deux joueurs pour savoir lequel afficher devant l'autre.
    // TODO: bool IsAbove(DinoPlayer& other);
};