/// @file dino_player.h
/// @brief Un acteur controllable par un utilisateur.

#pragma once

#include <dino/dino_actor.h>
#include <dino/dino_lasso.h>

#include <dino/xdino.h>
#include <dino/dino_animation.h>

/// Joueur dinosaure
/// @brief Un acteur controllable par un utilisateur.
class DinoPlayer : public DinoActor {
    friend DinoLasso;

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
    DinoVec2 lastPosition;
    AnimatorState<AnimationId> animatorState = {0, 0, IDLE};

public:
    /// Initialise le joueur.
    /// @param position Position de départ du joueur.
    /// @param gamepadIdx Indentificateur du gamepad lié au joueur.
    /// @param color Couleur du dinosaure.
    explicit DinoPlayer(DinoVec2 position, DinoGamepadIdx gamepadIdx, Color color)
        : DinoActor(position), gamepadIdx(gamepadIdx), direction(false), hitTimer(0), color(color),
          lastPosition(position)
    {
    }

    void update(float deltaTime) override;
    void draw() const override;
    /// Frappe le joueur et l'immobilise.
    void hit();

    DinoColor getColor() const;

private:
    void updateHit(float deltaTime);
    void updateMovement(float deltaTime, const DinoGamepad& gamepad);
    void updateAnimator(const DinoGamepad& gamepad);
    void updateAnimation(float deltaTime);
    void setAnimation(AnimationId animation);

    bool canMove() const;
};