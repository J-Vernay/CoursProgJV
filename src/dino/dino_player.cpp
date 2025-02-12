#include "dino_player.h"
#include "dino/dino_draw_utils.h"

constexpr float WALK_SPEED = 50.f;
constexpr float RUN_SPEED = 100.f;

const Animation animations[] = {
    {8, {0, 1, 2, 3}},
    {8, {4, 5, 6, 7, 8, 9}},
    {8, {14, 15, 16}},
    {16, {17, 18, 19, 20, 21, 22}}
};

void Player::init(DinoVec2 startPosition)
{
    this->position = startPosition;
}

void Player::update(float deltaTime)
{
    DinoGamepad gamepad;
    XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad);

    updateMovement(deltaTime, gamepad);
    updateAnimator(gamepad);
    updateAnimation(deltaTime);
}

void Player::draw() const
{
    DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(animations[animatorState.animationId].frames[animatorState.frame] * 24, 0, 24, 24, direction);
    drawCall.textureName = "dinosaurs.png";
    drawCall.translation = position;
    drawCall.scale = 2;
    XDino_Draw(drawCall);
}

void Player::updateMovement(float deltaTime, DinoGamepad gamepad)
{
    float speed = gamepad.btn_right ? RUN_SPEED : WALK_SPEED;

    position.x += gamepad.stick_left_x * speed * deltaTime;
    position.y += gamepad.stick_left_y * speed * deltaTime;
    
    if (abs(gamepad.stick_left_x) > 0.1f) {
        direction = gamepad.stick_left_x < 0;
    }
}

void Player::updateAnimator(DinoGamepad gamepad)
{
    if (abs(gamepad.stick_left_x) + abs(gamepad.stick_left_y) < 0.1f) {
        setAnimation(IDLE);
        return;
    }

    setAnimation(gamepad.btn_right ? RUN : WALK);
}

void Player::updateAnimation(float deltaTime)
{
    animatorState.timer += deltaTime;

    if (animatorState.timer < 1.0f / animations[animatorState.animationId].framerate) return;
    animatorState.frame = (++animatorState.frame) % animations[animatorState.animationId].frames.size();
    animatorState.timer = 0;
}

void Player::setAnimation(AnimationId animation)
{
    if (animatorState.animationId == animation)
        return;
    animatorState.animationId = animation;
    animatorState.frame = 0;
}