#include "dino_entity.h"

#include <dino/xdino.h>

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
    void DinoPlayer::Init(DinoVec2 initm_pos, int32_t idxPlayer, DinoGamepadIdx idxGamepad, DinoColor lassoColor)
    void Update(float deltaTime) override;
    void Draw(double timeSinceStart) override;

    /// Ne dessine que le lasso.
    void DrawLasso();

    bool IsAbove(DinoPlayer& other);

};