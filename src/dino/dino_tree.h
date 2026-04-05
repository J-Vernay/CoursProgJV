#pragma once
#include <dino/xdino.h>
#include <dino/dino_entity.h>
#include <dino/dino_draw_utils.h>

class DinoTree : public DinoEntity {
private:
    int m_idxSeason;
    bool m_bWasLooped;
    float m_unusableTimer;

    static uint64_t s_texID;

    void ReactLimit(bool xChanged) override;
    void ReactLoop(double timeSinceStart, int score, DinoColor lassoColor) override;

public:
    DinoTree(DinoVec2 pos, int idxSeason, float timeToBeUnusable);
    void Update(double timeSinceStart, float deltaTime);
    void Draw(double timeSinceStart) override;
    bool WasLooped();
    int GetIdxSeason();

    static void InitTexture();
    static void ShutTexture();
};