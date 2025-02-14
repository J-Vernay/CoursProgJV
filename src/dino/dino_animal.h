/// @file dino_animal.h
/// @brief Un acteur d'animal qui se balade aléatoirement sur le terrain, capturable pas les lassos des dinosaures.

#pragma once
#include <dino/dino_actor.h>
#include <dino/dino_animation.h>
#include <dino/xdino.h>

/// Animal
/// @brief Un acteur d'animal qui se balade aléatoirement sur le terrain, capturable pas les lassos des dinosaures.
class DinoAnimal : public DinoActor {
public:
    /// Enum représentant le type d'animal de l'acteur.
    enum AnimalType {
        PIG_1, ///< Un cochon.
        PIG_2, ///< Un cochon boueux.
        COW_1, ///< Un boeuf.
        COW_2, ///< Une vache.
        SHEEP_1, ///< Un bouc.
        SHEEP_2, ///< Un mouton.
        OSTRICH_1, ///< Une autruche noire.
        OSTRICH_2, ///< Une autruce marron.
        MAX ///< Taille maximum de l'enum.
    };

private:
    enum AnimationId {
        WALK_SIDE,
        WALK_DOWN,
        WALK_UP,
        DEAD
    };

    DinoVec2 direction;
    AnimalType type;
    AnimatorState<AnimationId> animatorState;
    float appearTimer;

public:
    /// Le constructeur de l'acteur.
    /// @param position La position où placer l'acteur à son apparition.
    explicit DinoAnimal(DinoVec2 position)
        : DinoActor(position), type(static_cast<AnimalType>(XDino_RandomInt32(0, MAX - 1))),
          animatorState({0, 0, WALK_SIDE}), appearTimer(0)
    {
        choose_random_direction();
    }

    void update(float deltaTime) override;
    void draw() const override;

protected:
    void onTerrainCollide() override;

private:
    void update_appear(float deltaTime);
    void update_movement(float deltaTime);
    void update_animation(float deltaTime);
    void choose_random_direction();
};