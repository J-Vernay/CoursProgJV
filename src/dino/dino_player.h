
#include <dino/xdino.h>

/// Représente l'état d'un joueur.
class DinoPlayer {
private:
    DinoVec2 pos = {};
    bool bMirror = false;

    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;
    
    int m_idxPlayer = 0;
    DinoGamepadIdx m_idxGamepad;
public:
    /// Configure le joueur pour répondre à une manette en particulier.
    void Init(DinoVec2 initPos, int32_t idxPlayer, DinoGamepadIdx idxGamepad);

    /// Met à jour la position du joueur et sa logique d'animation.
    void UpdatePlayer(float deltaTime);

    /// Affiche le joueur à l'écran.
    void DrawPlayer(double timeSinceStart);

    /// Compare la position de deux joueurs pour savoir lequel afficher devant l'autre.
    bool IsAbove(DinoPlayer& other);

    /// Ajuste la position du joueur pour rester sur le terrain.
    /// @param a Position en haut à gauche du terrain
    /// @param b Position en bas à droite du terrain
    void ApplyTerrain(DinoVec2 a, DinoVec2 b);

    /// Récupérer et modifier les positions (nécessaires pour collisions).
    DinoVec2 GetPos();
    void SetPos(DinoVec2 newPos);
};