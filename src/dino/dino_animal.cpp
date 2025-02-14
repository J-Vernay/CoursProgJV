#include <dino/xdino.h>
#include <dino/dino_animal.h>

// Constantes.
constexpr float ANIMAL_SPEED = 40.f; // Nombre de pixels parcourus en une seconde.
constexpr DinoVec2 SCREEN_SIZE = {480, 360};

void DinoAnimal::Init(DinoVec2 initPos, int32_t idxAnimal)
{
    m_pos = initPos;
    m_dir = XDino_RandomUnitVec2();
    m_idxAnimal = idxAnimal;
}

void DinoAnimal::UpdateAnimal(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.

    // Déplacement de l'animal. Si il touche un bord, il reprends une direction aléatoire.
    if (m_pos.x += m_dir.x * ANIMAL_SPEED * deltaTime > 0 && m_pos.x + m_dir.x * ANIMAL_SPEED * deltaTime < SCREEN_SIZE.
        x)
        m_pos.x += m_dir.x * ANIMAL_SPEED * deltaTime;
    else {
        m_dir = XDino_RandomUnitVec2();
    }

    if (m_pos.y += m_dir.y * ANIMAL_SPEED * deltaTime > 0 && m_pos.y + m_dir.y * ANIMAL_SPEED * deltaTime < SCREEN_SIZE.
        y)
        m_pos.y += m_dir.y * ANIMAL_SPEED * deltaTime;
    else {
        m_dir = XDino_RandomUnitVec2();
    }

    if (m_dir.x != 0)
        m_bMirror = m_dir.x > 0;
}

void DinoAnimal::DrawAnimal(double timeSinceStart)
{
    // Copier-coller de l'ancien code.

    DinoDrawCall drawCall;
    drawCall.textureName = "animals.png";
    DinoVec2 posA = {-16, -32};
    DinoVec2 posB = {16, -32};
    DinoVec2 posC = {-16, 0};
    DinoVec2 posD = {16, 0};

    int idxFrame = int(timeSinceStart * 8) % 4;
    int u = 32 * idxFrame;
    int v = 0;

    // Quel animal ?
    u += m_idxAnimal * 32 * 4;

    // Quelle animation utiliser ?

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

    if (m_bMirror) {
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
    drawCall.translation = m_pos;
    XDino_Draw(drawCall);
}

void DinoAnimal::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    bool bTerrainCollide = false;
    if (m_pos.y < a.y) {
        m_pos.y = a.y;
        bTerrainCollide = true;
    }
    if (m_pos.y > b.y) {
        m_pos.y = b.y;
        bTerrainCollide = true;
    }
    if (m_pos.x < a.x) {
        m_pos.x = a.x;
        bTerrainCollide = true;
    }
    if (m_pos.x > b.x) {
        m_pos.x = b.x;
        bTerrainCollide = true;
    }
    if (bTerrainCollide)
        m_dir = XDino_RandomUnitVec2();
}

/* TODO
bool DinoAnimal::IsAbove(DinoPlayer& other)
{
    return pos.y < other.pos.y;
}
*/