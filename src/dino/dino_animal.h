#pragma once
#include <vector>
#include "xdino.h"
#include "dino_terrain.h"

enum class EAnimalKind : int {
    Pig1,
    Pig2,
    Cow1,
    Cow2,
    Sheep1,
    Sheep2,
    Ostrich1,
    Ostrich2
};

enum class EAnimalAnim : int {
    Up,
    Down,
    Left,
    Right
};

/// Produit une liste de sommets qui correspond à un animal animé.
void Dino_GenVertices_Animal(
    std::vector<DinoVertex>& out,
    EAnimalKind kind,
    EAnimalAnim anim,
    double time
);

struct Animal {
    DinoVec2 pos;
    DinoVec2 targetPos;
    EAnimalKind kind;
    EAnimalAnim anim;

    double spawnTime;
    float alpha = 0;
};

class DinoAnimals {
public:
    void Init();

    void TrySpawn(double time, const DinoTerrain& terrain);

    void Update(double time, float deltaTime, const DinoTerrain& terrain);

    void Draw(double time) const;
    void Shut();

    void RepulseAnimals();

private:
    std::vector<Animal> animals;

    uint64_t animalsTex = 0;

    double lastSpawnTime = 0;

    static constexpr double ANIMAL_SPAWN_INTERVAL = 1;
    static constexpr double SPAWN_FADE_TIME = 0.5;

    static constexpr float SPRITE_SIZE = 32;
    static constexpr int NUM_ANIMALS = 8;
};