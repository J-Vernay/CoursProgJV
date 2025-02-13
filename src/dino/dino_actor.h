#pragma once
#include "xdino.h"

class DinoActor {
protected:
    DinoVec2 position;

public:
    explicit DinoActor(DinoVec2 position) : position(position)
    {
    }

    virtual void update(float deltaTime) {}
    virtual void draw() const {}

    static bool compareHeight(const DinoActor* first, const DinoActor* second);
};