#include <dino/xdino.h>
#include "dino_animal.h"

// Constantes.
constexpr float ANIMAL_SPEED = 40.f; // Nombre de pixels parcourus en une seconde.

void dino_animal::Init(DinoVec2 initPos, int32_t indexAnimal)
{
    pos = initPos;
    direction = XDino_RandomUnitVec2();
    indexAnimal = indexAnimal;
}

void dino_animal::UpdateAnimal(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.

    pos.x += direction.x * ANIMAL_SPEED * deltaTime;
    pos.y += direction.y * ANIMAL_SPEED * deltaTime;

    if (direction.x != 0)
        isMirror = direction.x > 0;
}

void dino_animal::DrawAnimal(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "animals.png";
    DinoVec2 posA = {0, 0};
    DinoVec2 posB = {32, 0};
    DinoVec2 posC = {0, 32};
    DinoVec2 posD = {32, 32};
    int idxFrame = int(timeSinceStart * 8) % 4;
    int u = 32 * idxFrame;
    int v = 0;
    // Quel animal ?
    u += idxAnimal * 32 * 4;

    // Quelle animation utiliser ?

    float fAbsoluDirX = direction.x;
    if (fAbsoluDirX < 0)
        fAbsoluDirX *= -1;
    float fAbsoluDirY = direction.y;
    if (fAbsoluDirY < 0)
        fAbsoluDirY *= -1;
    if (fAbsoluDirX > fAbsoluDirY) {
        // Le mouvement est surtout horizontal
        v = 0;
    }
    else {
        // Le mouvement est surtout vertical
        if (direction.y > 0)
            v = 32; // Vers le bas
        else
            v = 64; // Vers le haut
    }
    if (isMirror) {
        drawCall.vertices.emplace_back(posA, u + 32, v + 0);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 32, v + 32);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 32, v + 32);
        drawCall.vertices.emplace_back(posD, u + 0, v + 32);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, v + 0);
        drawCall.vertices.emplace_back(posB, u + 32, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 32);
        drawCall.vertices.emplace_back(posB, u + 32, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 32);
        drawCall.vertices.emplace_back(posD, u + 32, v + 32);
    }

    drawCall.scale = 1;
    drawCall.translation = pos;
    XDino_Draw(drawCall);
}

bool dino_animal::IsAbove(dino_animal& other)
{
    return pos.y < other.pos.y;
}