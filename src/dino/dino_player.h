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

    DinoVec2 position = {};
    bool direction = false;
    float hitTimer = 0;
    AnimatorState<AnimationId> animatorState = {0, 0, IDLE};

public:
    /// Initialise le joueur.
    /// @param startPosition Position de départ du joueur.
    void init(DinoVec2 startPosition);

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