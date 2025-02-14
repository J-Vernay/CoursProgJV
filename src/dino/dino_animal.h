
#include <dino/xdino.h>

#include <dino/dino_entity.h>

/// Représente l'état d'un animal.
class DinoAnimal : public DinoEntity {
private:
    DinoVec2 m_dir = {};
    int32_t m_idxAnimal;
    bool m_bMirror = false;

    void OnTerrainBorder() override;
    
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