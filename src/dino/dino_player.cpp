#include <dino/dino_player.h>
#include <dino/dino_movement.h>

void DinosaurPlayer::DinoMove(float xMovement, float yMovement)
{
    if (xMovement != 0 || yMovement != 0)
    {
        animCounter ++;
        if (animCounter > timeUntilAnimFrameChange) {
            animCounter = 0;
            animWalkIndicatorCounter++;

            if ((animWalkIndicatorCounter < 10)) {
                drawCallDino.vertices[0].u += 24;
                drawCallDino.vertices[1].u += 24;
                drawCallDino.vertices[2].u += 24;
                drawCallDino.vertices[3].u += 24;
                drawCallDino.vertices[4].u += 24;
                drawCallDino.vertices[5].u += 24;
            }
            else {
                animWalkIndicatorCounter = 1;
                drawCallDino.vertices[0].u -= 192;
                drawCallDino.vertices[1].u -= 192;
                drawCallDino.vertices[2].u -= 192;
                drawCallDino.vertices[3].u -= 192;
                drawCallDino.vertices[4].u -= 192;
                drawCallDino.vertices[5].u -= 192;
            }
            if (xMovement < 0) {
                if (!facingLeft) {
                    facingLeft = true;
                    SwitchDinoRotation(&drawCallDino);
                }
            }else if (facingLeft == true)
            {
                facingLeft = false;
                SwitchDinoRotation(&drawCallDino);
            }
        }
        lastDirection = xMovement;
    }
    else {
        InstantiateDinoVerticles(&drawCallDino);
        animWalkIndicatorCounter = 0;
        animCounter = 0;
        if(lastDirection < 0)SwitchDinoRotation(&drawCallDino);
    }

    drawCallDino.textureName = "dinosaurs.png";
        
    //Dino_CreateDrawCall_Circle(20);
    drawCallDino.translation = dinoPos;
    drawCallDino.scale = 2.5f;
    XDino_Draw(drawCallDino);
}
