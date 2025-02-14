#pragma once
#include "Entity.h"
#include "xdino.h"
#include <vector>


class PolyLine : public Entity
{
public:
    explicit PolyLine();
    explicit PolyLine(int size);

    void Update(float deltaTime) override;
    void Draw() override;
    
private:
    DinoColor POLYLINE_COLOR = {{70, 70, 100, 255}};
    std::vector<DinoVec2> polyline;
};
