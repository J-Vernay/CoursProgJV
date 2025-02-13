#include <dino/xdino.h>

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
    void Init(DinoVec2 initPos, int32_t idxPlayer, DinoGamepadIdx idxGamepad);
    void UpdatePlayer(float deltaTime);
    void DrawPlayer(double timeSinceStart);

    bool IsAbove(DinoPlayer& other);
};