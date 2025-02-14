#pragma once


#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

class DinoSpriteHandler {

public:

private:
    void ReadSprite(std::string textureName, DinoVec2 sPos, DinoVec2 ePos, DinoVec2 uv, DinoVec2 translate,
                    float scale);

    void ReadAnimatedSprite(std::string textureName, DinoVec2 pos, DinoVec2 uv, DinoVec2 translate, float scale,
                            float animationIndex, int frameCount, int timeSinceStart);
};