#include "DinoPlayer.h"

#include "dino_draw_utils.h"

#include <algorithm>

DinoPlayer::DinoPlayer()
{
    dinos.resize(4);
}

void DinoPlayer::Init()
{
    const Dino d(DinoGamepadIdx::Gamepad1, "dinosaurs.png", 0, {0, 0});
    dinos[0] = d;
    dinos[1] = Dino(DinoGamepadIdx::Keyboard, "dinosaurs.png", 1, {10, 10});
    dinos[2] = Dino(DinoGamepadIdx::Gamepad2, "dinosaurs.png", 2, {15, 15});
    dinos[3] = Dino(DinoGamepadIdx::Gamepad3, "dinosaurs.png", 3, {20, 20});
}

void DinoPlayer::Update(const float deltaTime) const
{
    for (auto d : dinos)
    {
        d.Update(deltaTime);
    }
}

void DinoPlayer::Draw()
{
    std::ranges::sort(dinos, Dino::CompareHeight);
    for (const auto& di : dinos)
    {
        di.Draw();
    }
}
