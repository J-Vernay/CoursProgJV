#pragma once
#include "xdino.h"


class DinoScore {

    int m_score = 0;
    int m_player = 0;
    DinoColor m_color = DinoColor_WHITE;

public:
    bool isInGame = false;

    void Init(int player, DinoColor m_color);
    void Reset();

    void AddScore(int score);
    void DrawScore();
};