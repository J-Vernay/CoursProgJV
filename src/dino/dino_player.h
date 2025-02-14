
#include <dino/xdino.h>

#include <dino/dino_entity.h>

/// Représente l'état d'un joueur.
class DinoPlayer : public DinoEntity {
private:
    bool bMirror = false;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;
    
    int m_idxPlayer = 0;
    DinoGamepadIdx m_idxGamepad;

    std::vector<DinoVec2> m_lasso;
    DinoColor m_lassoColor;
    
public:
    /// Configure le joueur pour répondre à une manette en particulier.
    void Init(DinoVec2 initPos, int32_t idxPlayer, DinoGamepadIdx idxGamepad, DinoColor lassoColor);

    /// Met à jour la position du joueur et sa logique d'animation.
    void Update(float deltaTime) override;

    /// Ne dessine que le lasso.
    void DrawLasso();
    
    /// Affiche le joueur à l'écran.
    void Draw(double timeSinceStart) override;

    /// Compare la position de deux joueurs pour savoir lequel afficher devant l'autre.
    bool IsAbove(DinoPlayer& other);

};