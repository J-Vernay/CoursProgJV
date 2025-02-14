#pragma once
#include "Animal.h"

#include <vector>

class AnimalPool
{
public:
    explicit AnimalPool();
    explicit AnimalPool(int size);

    void Init();
    void Update(float deltaTime);
    void Draw() const;
    
private:
    int _size;
    std::vector<Animal*> _animals;

    float _timer;

    void SpawnAnimal() const;
};

