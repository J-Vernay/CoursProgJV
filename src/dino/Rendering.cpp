#include "Rendering.h"
#include <algorithm>



std::vector<SpriteRenderer*> Rendering::spriteRenderer = {};

void Rendering::Update(float deltaTime)
{
    std::sort(spriteRenderer.begin(), spriteRenderer.end(),
        [](SpriteRenderer* firstSpriteRenderer, SpriteRenderer* secondSpriteRenderer)
        {
            return firstSpriteRenderer->GetPosition().y < secondSpriteRenderer->GetPosition().y;
        });

    for (int index = 0; index < spriteRenderer.size(); index++)
    {
        spriteRenderer[index]->Draw();
    }
}