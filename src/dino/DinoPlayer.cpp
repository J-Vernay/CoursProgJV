#include "DinoPlayer.h"

#include "dino_draw_utils.h"

#include <algorithm>
#include <iostream>

DinoPlayer::DinoPlayer()
{
    dinos.resize(4);
}

void DinoPlayer::Init()
{
    const auto d1 = new Dino(DinoGamepadIdx::Keyboard, "dinosaurs.png", 0, {480, 360});
    const auto d2 = new Dino(DinoGamepadIdx::Gamepad1, "dinosaurs.png", 1, {480, 360});
    const auto d3 = new Dino(DinoGamepadIdx::Gamepad2, "dinosaurs.png", 2, {480, 360});
    const auto d4 = new Dino(DinoGamepadIdx::Gamepad3, "dinosaurs.png", 3, {480, 360});
    
    dinos[0] = d1;
    dinos[1] = d2;
    dinos[2] = d3;
    dinos[3] = d4;
}

void DinoPlayer::Update(const float deltaTime) const
{
    for (auto const& d : this->dinos)
    {
        d->Update(deltaTime);
    }
}

void DinoPlayer::Draw()
{
    std::ranges::sort(dinos, Dino::CompareHeight);
    for (auto const& d : this->dinos)
    {
        d->Draw();
    }
}
