#include "AnimalPool.h"

AnimalPool::AnimalPool()
{
    _size = 0;
    _timer = 0;
}

AnimalPool::AnimalPool(const int size)
{
    _size = size;
    _timer = 0;
}

void AnimalPool::Init()
{
    for (int i = 0; i < _size; i++)
    {
        const auto u = XDino_RandomUint16(0, 3);
        
        const auto animal = new Animal("animals.png", i, {0, 0}, 8, u * 32, 32);
        _animals.emplace_back(animal);
    }
}

void AnimalPool::Update(const float deltaTime)
{
    _timer += deltaTime;

    if (_timer >= 1)
    {
        _timer = 0;
        SpawnAnimal();
    }
    
    for (const auto& animal : _animals)
    {
        animal->Update(deltaTime);
    }
}

void AnimalPool::Draw() const
{
    for (const auto& animal : _animals)
    {
        animal->Draw();
    }
}

void AnimalPool::SpawnAnimal() const
{
    for (const auto& animal : _animals)
    {
        if (animal->GetActive())
            continue;

        animal->Spawn();
        return;
    }
}

