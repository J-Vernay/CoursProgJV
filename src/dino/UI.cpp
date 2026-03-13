#include "UI.h"

#include <format>

void UI::Update(float deltatime)
{
    HandleChronometer(deltatime);
}

void UI::HandleChronometer(float deltatime)
{
    chronometer -= deltatime;

    {
        std::string text = std::format("{}", static_cast<int>(chronometer));
        
        std::vector<DinoVertex> vs;
        
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");

        
        float tx = XDino_GetRenderSize().x/2;
        float ty = textSize.y;
        
        XDino_Draw(vbufID, XDino_TEXID_FONT, {tx,ty}, 2);
        
        XDino_DestroyVertexBuffer(vbufID);
    }
}
