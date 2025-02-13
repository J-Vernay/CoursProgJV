/// @file dino_player.h
/// @brief Un acteur controllable par un utilisateur.

#pragma once

#include "dino_actor.h"
#include "dino_lasso.h"

#include <dino/xdino.h>
#include <dino/dino_animation.h>

/// Joueur dinosaure
/// @brief Un acteur controllable par un utilisateur.
class DinoPlayer : public DinoActor {
public:
    /// Couleur d'un dinosaure
    enum Color {
        BLUE,
        RED,
        YELLOW,
        GREEN
    };
private:
    enum AnimationId {
        IDLE,
        WALK,
        HIT,
        RUN
    };

    DinoLasso lasso;
    DinoGamepadIdx gamepadIdx;
    bool direction;
    float hitTimer;
    Color color;
    AnimatorState<AnimationId> animatorState = {0, 0, IDLE};

public:
    /// Initialise le joueur.
    /// @param position Position de départ du joueur.
    /// @param gamepadIdx Indentificateur du gamepad lié au joueur.
    /// @param color Couleur du dinosaure.
    explicit DinoPlayer(DinoVec2 position, DinoGamepadIdx gamepadIdx, Color color)
        : DinoActor(position), gamepadIdx(gamepadIdx), direction(false), hitTimer(0), color(color)
    {
    }
    
    void update(float deltaTime) override;
    void draw() const override;
    void drawLasso() const;

    /// Frappe le joueur et l'immobilise.
    void hit();
private:
    void updateHit(float deltaTime);
    void updateMovement(float deltaTime, const DinoGamepad& gamepad);
    void updateAnimator(const DinoGamepad& gamepad);
    void updateAnimation(float deltaTime);
    void setAnimation(AnimationId animation);

    bool canMove() const;
};