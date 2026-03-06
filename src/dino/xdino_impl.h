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

int XDinoImpl_DrawStats(int scroll, int maxlines, float scale, std::vector<std::string> lines)
{
    if (scroll < 0)
        scroll = 0;
    int lineCount = static_cast<int>(lines.size());
    if (maxlines > lineCount)
        maxlines = lineCount;
    if (scroll >= lineCount - maxlines)
        scroll = lineCount - maxlines;

    DinoVec2 pos = {};
    for (int i = 0; i < maxlines; ++i) {
        DinoVec2 size;
        DinoDrawCall d = Dino_CreateDrawCall_Text(lines[scroll + i], DinoColor_WHITE, DinoColor_BLACK, &size);
        d.translation = pos;
        d.scale = scale;
        XDino_Draw(std::move(d));
        pos.y += size.y * scale;
        XDino_DestroyVertexBuffer(d.vbufID);
    }
    return scroll;
}