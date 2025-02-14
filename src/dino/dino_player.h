#include "dino_entity.h"

#include <dino/xdino.h>

class DinoPlayer : DinoEntity {
private:
    bool bMirror = false;
    bool bIdle = false;
    bool bWalking = false;
    bool bRunning = false;

    int m_idxPlayer = 0;
    DinoGamepadIdx m_idxGamepad;

public:
    void Init(DinoVec2 initPos, int32_t idxPlayer, DinoGamepadIdx idxGamepad);
    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);

    bool IsAbove(DinoPlayer& other);

    void ApplyTerrain(DinoVec2 a, DinoVec2 b);
    
};