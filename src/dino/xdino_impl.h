#pragma once

/// Trucs communs entre les différentes implémentations de l'API XDino.
/// En général, cela concerne le debug interne.
/// Rien à voir pour vous autres programmeurs côté gameplay :)

#ifndef XDINO_IMPL
#error "xdino_impl.h doit être inclus par l'implémentation plateforme, pas côté gameplay."
#endif

#include <string>
#include <vector>

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

int XDinoImpl_DrawStats(int scroll, float maxY, float scale, std::vector<std::string> lines)
{
    if (scroll < 0)
        scroll = 0;
    int lineCount = static_cast<int>(lines.size());
    if (scroll >= lineCount)
        scroll = lineCount;

    DinoVec2 pos = {};
    for (int i = scroll; i < lineCount; ++i) {
        std::vector<DinoVertex> vs;
        DinoVec2 size = Dino_GenVertices_Text(vs, lines[i], DinoColor_WHITE, DinoColor_BLACK);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs, "DrawStats");
        XDino_Draw(vbufID, XDino_TEXID_FONT, pos, scale);
        XDino_DestroyVertexBuffer(vbufID);
        pos.y += size.y * scale;
        if (pos.y >= maxY)
            break;
    }
    return scroll;
}