#pragma once

#include <dino/xdino.h>
#include <dino/dino_animation.h>

/// Representation et logique d'un joueur
class DinoPlayer {
    enum AnimationId {
        IDLE,
        WALK,
        HIT,
        RUN
    };

    DinoGamepadIdx gamepadIdx;
    DinoVec2 position = {};
    bool direction = false;
    float hitTimer = 0;
    int16_t color = 0;
    AnimatorState<AnimationId> animatorState = {0, 0, IDLE};

public:
    /// Initialise le joueur.
    /// @param position Position de départ du joueur.
    explicit DinoPlayer(DinoVec2 position, DinoGamepadIdx gamepadIdx, int16_t color) : gamepadIdx(gamepadIdx),
        position(position), color(color)
    {
    }

    /// Effectue la logique de jeu du joueur.
    /// @param deltaTime Temps passé depuis la dernière frame.
    void update(float deltaTime);

    /// Render le joueur à l'écran.
    void draw() const;

    /// Frappe le joueur et l'immobilise.
    void hit();

private:
    void updateHit(float deltaTime);
    void updateMovement(float deltaTime, DinoGamepad gamepad);
    void updateAnimator(DinoGamepad gamepad);
    void updateAnimation(float deltaTime);
    void setAnimation(AnimationId animation);

    bool canMove();
};