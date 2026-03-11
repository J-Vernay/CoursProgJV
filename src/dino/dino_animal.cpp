#include "dino_animal.h"
#include <cmath>
#include <algorithm>

void DinoAnimals::Init()
{
    animals.clear();
    lastSpawnTime = 0;

    animalsTex = XDino_CreateGpuTexture("animals.png");
}

void DinoAnimals::TrySpawn(double time, const DinoTerrain& terrain)
{
    if (time - lastSpawnTime >= ANIMAL_SPAWN_INTERVAL) {
        lastSpawnTime = time;

        Animal a;

        // Coin supérieur gauche du sprite doit rester dans le terrain
        float minX = terrain.GetOrigin().x;
        float minY = terrain.GetOrigin().y - (SPRITE_SIZE / 2);

        float maxX = terrain.GetOrigin().x + terrain.GetWidth() - SPRITE_SIZE;
        float maxY = terrain.GetOrigin().y + terrain.GetHeight() - SPRITE_SIZE;

        a.pos.x = (float)(XDino_RandomInt32((int)minX, (int)maxX));
        a.pos.y = (float)(XDino_RandomInt32((int)minY, (int)maxY));

        a.targetPos = a.pos;

        a.kind = (EAnimalKind)(XDino_RandomInt32(0, NUM_ANIMALS - 1));

        a.anim = EAnimalAnim::Down;

        a.spawnTime = time;

        a.alpha = 0;

        animals.push_back(a);
    }
}

void DinoAnimals::Update(double time, float deltaTime, const DinoTerrain& terrain)
{
    float minX = terrain.GetOrigin().x;
    float minY = terrain.GetOrigin().y - (SPRITE_SIZE / 2);

    float maxX = terrain.GetOrigin().x + terrain.GetWidth() - SPRITE_SIZE;
    float maxY = terrain.GetOrigin().y + terrain.GetHeight() - SPRITE_SIZE;

    for (Animal& a : animals) {
        double t = time - a.spawnTime;

        a.alpha = (float)(std::min(t / SPAWN_FADE_TIME, 1.0));

        float speed = 30;

        DinoVec2 dir = {
            a.targetPos.x - a.pos.x,
            a.targetPos.y - a.pos.y
        };

        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < 0.5f) {
            a.targetPos.x = (float)(XDino_RandomInt32((int)minX, (int)maxX));
            a.targetPos.y = (float)(XDino_RandomInt32((int)minY, (int)maxY));

            dir = {
                a.targetPos.x - a.pos.x,
                a.targetPos.y - a.pos.y
            };

            dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        }

        if (dist > 0.01f) {
            float moveDist = speed * deltaTime;

            a.pos.x += dir.x / dist * moveDist;
            a.pos.y += dir.y / dist * moveDist;

            a.pos.x = std::clamp(a.pos.x, minX, maxX);
            a.pos.y = std::clamp(a.pos.y, minY, maxY);
        }

        if (std::abs(dir.x) > std::abs(dir.y)) {
            a.anim = dir.x > 0 ? EAnimalAnim::Right : EAnimalAnim::Left;
        }
        else {
            a.anim = dir.y > 0 ? EAnimalAnim::Down : EAnimalAnim::Up;
        }
    }
}

void DinoAnimals::Draw(double time) const
{
    for (const Animal& a : animals) {
        std::vector<DinoVertex> vs;

        Dino_GenVertices_Animal(vs, a.kind, a.anim, time);

        for (DinoVertex& v : vs) {
            v.color.a = (uint8_t)(v.color.a * a.alpha);
        }

        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");

        XDino_Draw(vbufID, animalsTex, a.pos, 1, 0);

        XDino_DestroyVertexBuffer(vbufID);
    }
}

void DinoAnimals::Shut()
{
    XDino_DestroyGpuTexture(animalsTex);
}


void Dino_GenVertices_Animal(
    std::vector<DinoVertex>& out, EAnimalKind kind, EAnimalAnim anim, double time)
{
    int idxFrame = (int)(time * 8) % 4;
    uint16_t uMin = (int)kind * 128 + idxFrame * 32;
    uint16_t uMax = uMin + 32;
    uint16_t vMin = 0;

    if (anim == EAnimalAnim::Up) {
        vMin = 64;
    }
    else if (anim == EAnimalAnim::Down) {
        vMin = 32;
    }
    else if (anim == EAnimalAnim::Left) {
        vMin = 0;
    }
    else if (anim == EAnimalAnim::Right) {
        vMin = 0;
        uint16_t tmp = uMin;
        uMin = uMax;
        uMax = tmp;
    }
    uint16_t vMax = vMin + 32;

    int i = out.size();
    out.resize(i + 6);

    out[i].pos = {};
    out[i].u = uMin;
    out[i].v = vMin;
    out[i + 1].pos = {32, 0};
    out[i + 1].u = uMax;
    out[i + 1].v = vMin;
    out[i + 2].pos = {0, 32};
    out[i + 2].u = uMin;
    out[i + 2].v = vMax;

    out[i + 3].pos = {32, 0};
    out[i + 3].u = uMax;
    out[i + 3].v = vMin;
    out[i + 4].pos = {0, 32};
    out[i + 4].u = uMin;
    out[i + 4].v = vMax;
    out[i + 5].pos = {32, 32};
    out[i + 5].u = uMax;
    out[i + 5].v = vMax;
}

void DinoAnimals::RepulseAnimals()
{
    float radius = SPRITE_SIZE / 3.0f;
    float minDistance = radius * 2.0f;

    for (size_t i = 0; i < animals.size(); ++i) {
        for (size_t j = i + 1; j < animals.size(); ++j) {
            DinoVec2 dir = {animals[j].pos.x - animals[i].pos.x,
                            animals[j].pos.y - animals[i].pos.y};
            float dist2 = dir.x * dir.x + dir.y * dir.y;

            if (dist2 == 0.0f) {
                dir.x = 1.0f;
                dir.y = 0.0f;
                dist2 = 1.0f;
            }

            if (dist2 < minDistance * minDistance) {
                float dist = std::sqrt(dist2);
                float overlap = (minDistance - dist) / 2.0f;
                animals[i].pos.x -= dir.x / dist * overlap;
                animals[i].pos.y -= dir.y / dist * overlap;
                animals[j].pos.x += dir.x / dist * overlap;
                animals[j].pos.y += dir.y / dist * overlap;
            }
        }
    }
}