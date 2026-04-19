#include "FloatingText.h"
#include <dino/dino_draw_utils.h>
#include <vector>

FloatingText::FloatingText(DinoVec2 spawnPosition, std::string displayText, DinoColor textColor, float lifetime)
    : currentPosition(spawnPosition), displayText(displayText), textColor(textColor), lifetime(lifetime)
{}

bool FloatingText::IsExpired() const
{
    return elapsed >= lifetime;
}

void FloatingText::Update(float deltaTime)
{
    elapsed += deltaTime;
    currentPosition.y -= 30.f * deltaTime;
}

void FloatingText::Draw() const
{
    std::vector<DinoVertex> vertices;
    Dino_GenVertices_Text(vertices, displayText, textColor, DinoColor_TRANSPARENT);
    DinoVertexBuffer vertexBuffer(vertices.data(), vertices.size(), "FloatingText");
    XDino_Draw(vertexBuffer.Get(), XDino_TEXID_FONT, currentPosition, 2);
}