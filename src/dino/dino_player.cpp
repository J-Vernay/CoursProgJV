/// @file dino_player.cpp
/// @brief Implémentation de `DinoPlayer`.

#include <dino/dino_player.h>
#include <dino/dino_draw_utils.h>

constexpr float WALK_SPEED = 75.f;
constexpr float RUN_SPEED = 150.f;
constexpr float HIT_TIME = 1.5f;

const Animation animations[] = {
    {8, {0, 1, 2, 3}},
    {8, {4, 5, 6, 7, 8, 9}},
    {8, {14, 15, 16}},
    {16, {17, 18, 19, 20, 21, 22}}
};

constexpr DinoColor colors[] = {
    {{77, 146, 188, 255}},
    {{188, 77, 79, 255}},
    {{253, 199, 96, 255}},
    {{159, 188, 77, 255}}
};

void DinoPlayer::update(float deltaTime)
{
    DinoActor::update(deltaTime);

    DinoGamepad gamepad;
    XDino_GetGamepad(gamepadIdx, gamepad);

    updateHit(deltaTime);

    if (canMove()) {
        updateMovement(deltaTime, gamepad);
    }
    updateAnimator(gamepad);
    updateAnimation(deltaTime);
}

void DinoPlayer::draw() const
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(
        animations[animatorState.animationId].frames[animatorState.frame],
        color,
        24,
        24,
        {-12, -12},
        direction);
    drawCall.textureName = "dinosaurs.png";
    drawCall.translation = position;
    XDino_Draw(drawCall);
}

void DinoPlayer::hit()
{
    hitTimer = HIT_TIME;
}

DinoColor DinoPlayer::getColor() const
{
    return colors[color];
}

void DinoPlayer::updateHit(float deltaTime)
{
    if (hitTimer <= 0)
        return;
    hitTimer -= deltaTime;
}

void DinoPlayer::updateMovement(float deltaTime, const DinoGamepad& gamepad)
{
    float speed = gamepad.btn_right ? RUN_SPEED : WALK_SPEED;

    position.x += gamepad.stick_left_x * speed * deltaTime;
    position.y += gamepad.stick_left_y * speed * deltaTime;

    if (abs(gamepad.stick_left_x) > 0.1f) {
        direction = gamepad.stick_left_x < 0;
    }
}

void DinoPlayer::updateAnimator(const DinoGamepad& gamepad)
{
    if (hitTimer > 0) {
        setAnimation(HIT);
        return;
    }

    if (abs(gamepad.stick_left_x) + abs(gamepad.stick_left_y) < 0.1f) {
        setAnimation(IDLE);
        return;
    }

    setAnimation(gamepad.btn_right ? RUN : WALK);
}

void DinoPlayer::updateAnimation(float deltaTime)
{
    animatorState.timer += deltaTime;

    if (animatorState.timer < 1.0f / animations[animatorState.animationId].framerate)
        return;
    animatorState.frame = (++animatorState.frame) % animations[animatorState.animationId].frames.size();
    animatorState.timer = 0;
}

void DinoPlayer::setAnimation(AnimationId animation)
{
    if (animatorState.animationId == animation)
        return;
    animatorState.animationId = animation;
    animatorState.frame = 0;
}

bool DinoPlayer::canMove() const
{
    return hitTimer <= 0;
}