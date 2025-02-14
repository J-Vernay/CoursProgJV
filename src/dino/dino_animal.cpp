#include "dino_animal.h"

void dino_animal::InitAnimal(DinoVec2 initPos, int32_t idxAnimal)
{
    pos = initPos;
    m_dir = XDino_RandomUnitVec2();
    m_idxAnimal = idxAnimal;
}

void dino_animal::Update(float deltaTime)
{
    // Gestion de la position de l'animal.

    pos.x += m_dir.x * ANIMAL_SPEED * deltaTime;
    pos.y += m_dir.y * ANIMAL_SPEED * deltaTime;

    if (m_dir.x != 0)
        bMirror = m_dir.x > 0;
}

void dino_animal::Draw(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "animals.png";
    DinoVec2 posA = {-16, -32};
    DinoVec2 posB = {16, -32};
    DinoVec2 posC = {-16, 0};
    DinoVec2 posD = {16, 0};

    int idxFrame = static_cast<int>(timeSinceStart * 8) % 4;
    int u = 32 * idxFrame;
    int v = 0;

    u += m_idxAnimal * 128;

    float fAbsoluDirX = m_dir.x;
    if (fAbsoluDirX < 0)
        fAbsoluDirX *= -1;
    float fAbsoluDirY = m_dir.y;
    if (fAbsoluDirY < 0)
        fAbsoluDirY *= -1;
    if (fAbsoluDirX > fAbsoluDirY) {
        // Le mouvement est surtout horizontal
        v = 0;
    }
    else {
        // Le mouvement est surtout vertical
        if (m_dir.y > 0)
            v = 32; // Vers le bas
        else
            v = 64; // Vers le haut
    }

    if (bMirror) {
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


void dino_animal::OnTerrainBorder()
{
    m_dir = XDino_RandomUnitVec2();
}