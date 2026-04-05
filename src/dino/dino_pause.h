#pragma once
#include "xdino.h"

class DinoPause {
    
    static int m_currSelection;
    static DinoVec2 renderSize;
    static std::vector<std::string> optionNames;
    
public:
    
    static void Init(DinoVec2 rdrSize);
    
    static void Scroll(bool scrollingUp);
    static void ChangeTimer(bool scrollingLeft, double& timer, double timerInit);
    static int GetCurrSelection();
    
    static void DrawPauseMenu();
};