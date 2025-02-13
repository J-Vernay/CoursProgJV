#pragma once
#include "Dino.h"
#include <vector>

class DinoPlayer
{
public:
    explicit DinoPlayer();

    void Init();
    void Update(float deltaTime) const;
    void Draw();
    
private:
    std::vector<Dino> dinos;
};
