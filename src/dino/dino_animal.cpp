#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>

const std::string DinoAnimal::TEXTURE_NAME = "animals.png";

void DinoAnimal::InitRandom(DinoVec2 pos)
{
    m_pos = {pos.x - 16, pos.y - 32};
    m_kind = XDino_RandomInt32(0, 7);
    m_dir = XDino_RandomUnitVec2();
}

void DinoAnimal::Update(double timeSinceStart, float deltaTime)
{
    float speed = 30;
    m_pos.x += m_dir.x * deltaTime * speed;
    m_pos.y += m_dir.y * deltaTime * speed;
}

void DinoAnimal::AddDrawCall(double timeSinceStart, float deltaTime, DinoDrawCall& drawCall) const
{
    uint16_t u1 = 0, u2 = 32, v1 = 0, v2 = 32;

    if (m_dir.x >= 0) {
        // L'animal va vers la droite, et le sprite est orienté vers la gauche -> On inverse la coordonnée U du sprite.
        u1 = 32;
        u2 = 0;
    }

    if (fabsf(m_dir.x) > fabsf(m_dir.y)) {
        // L'animal se déplace surtout horizontalement.
        v1 = 0;
        v2 = 32;
    }
    else if (m_dir.y >= 0) {
        // L'animal va vers le bas.
        v1 = 32;
        v2 = 64;
    }
    else {
        // L'animal va vers le haut.
        v1 = 64;
        v2 = 96;
    }

    uint16_t idxFrame = static_cast<uint16_t>(timeSinceStart * 8) % 4;
    u1 += idxFrame * 32;
    u2 += idxFrame * 32;

    u1 += 128 * m_kind;
    u2 += 128 * m_kind;

    drawCall.vertices.emplace_back(DinoVec2{m_pos.x, m_pos.y}, u1, v1);
    drawCall.vertices.emplace_back(DinoVec2{m_pos.x + 32, m_pos.y}, u2, v1);
    drawCall.vertices.emplace_back(DinoVec2{m_pos.x, m_pos.y + 32}, u1, v2);
    drawCall.vertices.emplace_back(DinoVec2{m_pos.x + 32, m_pos.y}, u2, v1);
    drawCall.vertices.emplace_back(DinoVec2{m_pos.x, m_pos.y + 32}, u1, v2);
    drawCall.vertices.emplace_back(DinoVec2{m_pos.x + 32, m_pos.y + 32}, u2, v2);
}