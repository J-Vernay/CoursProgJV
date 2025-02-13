#pragma once
#include "xdino.h"
#include <vector>


class PolyLine
{
public:
    explicit PolyLine();
    explicit PolyLine(int size);

    void Update(float deltaTime);
    void Draw();
    
private:
    DinoColor POLYLINE_COLOR = {{70, 70, 100, 255}};
    std::vector<DinoVec2> polyline;
};
