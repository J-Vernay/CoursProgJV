#pragma once

#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_entity.h>

class DinoAnimal : public DinoEntity
{
    
public:
    void Update(float deltaTime) override;
    void Display(double timeSinceStart) override;
    void DinoAnimalInit(DinoVec2 pos, int idx);

    void OnTerrainBorder() override;

    
private:
    DinoVec2 dir_ = {};
    bool mirror_ = false;

    bool up_ = false;
    bool left_ = false;
    bool down_ = false;

    int sprite_idx_ = 0;
    int animal_type_ = 0;
};
