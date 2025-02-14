/// @file dino_animal.cpp
/// @brief Implémentation de la classe DinoAnimal.

#include <algorithm>
#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>

constexpr float SPEED = 50;
constexpr float APPEAR_TIME = 0.5f;

void DinoAnimal::update(float deltaTime)
{
    update_appear(deltaTime);
    update_movement(deltaTime);
    update_animation(deltaTime);
}

void DinoAnimal::draw() const
{
    DinoColor tintColor = {255, 255, 255,
                           static_cast<uint8_t>(std::clamp(appearTimer / APPEAR_TIME, 0.0f, 1.0f) * 255)};

    DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(
        type * 4 + animatorState.frame,
        animatorState.animationId,
        32,
        32,
        {-16, -24},
        direction.x > 0,
        tintColor);
    drawCall.textureName = "animals.png";
    drawCall.translation = position;
    XDino_Draw(drawCall);
}

void DinoAnimal::onTerrainCollide()
{
    choose_random_direction();
}

void DinoAnimal::update_appear(float deltaTime)
{
    if (appearTimer >= APPEAR_TIME)
        return;
    appearTimer += deltaTime;
}

void DinoAnimal::update_movement(float deltaTime)
{
    position = {position.x + direction.x * SPEED * deltaTime, position.y + direction.y * SPEED * deltaTime};
}

void DinoAnimal::update_animation(float deltaTime)
{
    animatorState.timer += deltaTime;
    if (animatorState.timer < 1 / 8.0f)
        return;
    animatorState.timer = 0;
    animatorState.frame = (animatorState.frame + 1) % 4;
}

void DinoAnimal::choose_random_direction()
{
    direction = XDino_RandomUnitVec2();

    if (abs(direction.x) > abs(direction.y)) {
        animatorState.animationId = WALK_SIDE;
    }
    else {
        animatorState.animationId = direction.y < 0 ? WALK_UP : WALK_DOWN;
    }
}