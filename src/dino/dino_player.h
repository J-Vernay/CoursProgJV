#pragma once
#include <dino/xdino.h>

#ifndef player
#define player

class DinosaurPlayer {
    public :
        DinoDrawCall drawCallDino;
        int dinoToChoose;
        int animWalkIndicatorCounter = 0;
    
        float speed = 150.f;
        float timeUntilDoubleSpeed = 1.2f;
        float timeUntilAnimFrameChange = 4;
        float counter, animCounter, lastDirection;
    
        bool facingLeft = false;
        DinoVec2 dinoPos = {};
    
        DinosaurPlayer(int yDinoToChoose): counter(0), animCounter(0), lastDirection(0)
        {dinoToChoose = yDinoToChoose;}

        void DinoMove(float xMovement, float yMovement);

    private :
};

#endif