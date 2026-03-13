#pragma once
#include <dino/xdino.h>
#include <dino/dino_entity.h>
#include <dino/dino_draw_utils.h>

class DinoTree : public DinoEntity {
private:
    int m_idxSeason;
    bool m_bWasLooped;

    static uint64_t s_texID;

    void ReactLimit() override;
    void ReactLoop(double timeSinceStart) override;

public:
    DinoTree(DinoVec2 pos, int idxSeason);
    void Update(double timeSinceStart, float deltaTime);
    void Draw(double timeSinceStart) override;
    bool WasLooped();
    int GetIdxSeason();


    static void InitStatic();
    static void ShutStatic();
};