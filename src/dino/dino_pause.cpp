#include <dino/dino_pause.h>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <algorithm>
#include <vector>

int DinoPause::m_currSelection = 0;
DinoVec2 DinoPause::renderSize;
std::vector<std::string> DinoPause::optionNames;

void DinoPause::Init(DinoVec2 rdrSize)
{
    renderSize = rdrSize;
    
    optionNames.resize(4);
    optionNames[0] = "Restart";
    optionNames[1] = "End Game";
    optionNames[2] = "<(-10) Timer (+10)>";
    optionNames[3] = "Resume";
}

void DinoPause::Scroll(bool scrollingUp)
{
    if (scrollingUp && m_currSelection > 0)
        m_currSelection--;
    
    if (!scrollingUp && m_currSelection < 3)
        m_currSelection++;
    
    // fail proof verification
    m_currSelection = std::clamp(m_currSelection,0, 3);
}

void DinoPause::ChangeTimer(bool scrollingLeft, double& timer, double timerInit)
{
    if (m_currSelection != 2)
        return;
    
    if (scrollingLeft)
        timer -= 10;
    else
        timer += 10;
    
    timer = std::clamp(timer, (double)0.01f, timerInit);
}

int DinoPause::GetCurrSelection()
{
    return m_currSelection; 
}

void DinoPause::DrawPauseMenu()
{
    DinoColor textColor = DinoColor_WHITE;
    DinoColor backgroundColor = DinoColor_BLACK;
    
    DinoVec2 textSize;
    float tx;
    float ty;
    
    // Title
    {
        std::vector<DinoVertex> vs;
        textSize = Dino_GenVertices_Text(vs, "---- PAUSE ----", textColor, backgroundColor);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "PauseMenu");
        tx = (renderSize.x - textSize.x * 3) / 2;
        ty = 40;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 3);
    }
    
    // Options
    for (int i = 0; i < optionNames.size(); i++) {
        
        if (i == m_currSelection) {
            textColor = DinoColor_BLACK;
            backgroundColor = DinoColor_WHITE;
        }
        else {
            textColor = DinoColor_WHITE;
            backgroundColor = DinoColor_BLACK;
        }
        
        {
            std::vector<DinoVertex> vs;
            textSize = Dino_GenVertices_Text(vs, optionNames[i], textColor, backgroundColor);
            DinoVertexBuffer vbuf(vs.data(), vs.size(), "PauseMenu");
            tx = (renderSize.x - textSize.x * 3) / 2;
            ty = 100 + (textSize.y + 2) * 3 * i;
            XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 3);
        }
    }
}