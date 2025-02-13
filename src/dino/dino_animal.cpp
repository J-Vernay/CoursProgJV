#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>

void DinoAnimal::update(float deltaTime)
{

}

void DinoAnimal::draw()
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(
        type * 4,
        0,
        32,
        32,
        {-16, -24},
        direction);
    drawCall.textureName = "animals.png";
    drawCall.translation = position;
    XDino_Draw(drawCall);
}