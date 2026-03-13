#pragma once

#include <dino/xdino.h>
#include "dino/CodeBase/dino_draw_utils.h"


class UI
{
public:
    float chronometer = 60.0f;
    float chronometerStart;



    void Update(float);


    void HandleChronometer(float);
    
};




