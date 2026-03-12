#pragma once

#include <dino/xdino.h>
#include <dino/dino_entity.h>

struct DinoControllerFields : public DinoEntity {

    // Fields
private:
    double m_dinoCurrentSpeed;
    bool m_dinoGoingLeft = false;

    float m_dinoAnimElapsed = 0;
    float m_dinoDamageAnimTimer;
    int m_currFrame = 0;
    int m_dinoColor = 0;

    bool m_dinoTakeDamage = false;

    DinoGamepad m_gamepad;

    // std::vector<DinoVec2> m_lasso;
    // DinoColor m_lassoColor = {77, 146, 188, 255};
    // float m_lassoLifeSpan = 2.f;
    // uint64_t vbufID_lasso;

public:
    uint64_t vbufID_dino;

    static uint64_t s_texID;

    // Methods
private:
    uint64_t GenDinoVertexBuffer(float timeSinceStart);
    void ReactLimit(bool xChanged) override;
    void ReactLoop(double timeSinceStart) override;

public:
    void Init(int playerCount);
    // void Shut();

    void DinoMovement(DinoGamepad gamepad, float deltaTime);
    void Draw(double timeSinceStart) override;
    // void DrawLasso();
    // void CheckForOwnLassoIntersections();
    // void CheckForOtherLassIntersections(DinoControllerFields& other);
    static void InitTexture();
    static void ShutTexture();
};