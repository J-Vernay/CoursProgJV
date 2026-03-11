#include <algorithm>
#include <dino/dino_animal.h>
#include <dino/dino_draw_utils.h>


#pragma region AnimalSpawner
void DinoAnimalSpawner::Init()
{
    m_texID = XDino_CreateGpuTexture("animals.png");
}

void DinoAnimalSpawner::Update(float deltaTime, double timeSinceStart)
{
    if (timeSinceStart - m_timeSinceLastSpawn > 1) {
        int animalIdx = XDino_RandomInt32(0, 7);

        m_animals.emplace_back();
        m_animals.back().Init(timeSinceStart, animalIdx, m_texID);

        m_timeSinceLastSpawn = timeSinceStart;
    }

    for (auto& animal : m_animals) {
        animal.Update(deltaTime, timeSinceStart);
        animal.Draw(timeSinceStart);
    }
}

void DinoAnimalSpawner::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}
#pragma endregion

#pragma region AnimalBehaviours
void DinoAnimal::Init(double timeSinceStart, int animalIndex, uint64_t texID)
{
    m_animalType = animalIndex;
    m_texID = texID;
    m_spawnTime = timeSinceStart;
    DinoVec2 rdrSize = XDino_GetRenderSize();
    m_posTopLeft = {
        (rdrSize.x - TERRAIN_SIZE.x) / 2,
        (rdrSize.y - TERRAIN_SIZE.y) / 2
    };
    m_pos = DinoVec2(m_posTopLeft.x + XDino_RandomFloat(2, TILE_COUNT.x) * 16 - 32,
                     m_posTopLeft.y + XDino_RandomFloat(1, TILE_COUNT.y) * 16 - 32
    );

    m_dir = XDino_RandomUnitVec2();
}

void DinoAnimal::Update(float deltaTime, double timeSinceStart)
{

    constexpr float SPEED = 100;
    m_pos.x += m_dir.x * SPEED * deltaTime;
    m_pos.y += m_dir.y * SPEED * deltaTime;

    float minX = m_posTopLeft.x + 12;
    float maxX = m_posTopLeft.x + TERRAIN_SIZE.x - 12;
    float minY = m_posTopLeft.y + 12;
    float maxY = m_posTopLeft.y + TERRAIN_SIZE.y - 12;

    bool hitBorder = false;

    if (m_pos.x < minX) {
        m_pos.x = minX;
        hitBorder = true;
    }
    if (m_pos.x > maxX) {
        m_pos.x = maxX;
        hitBorder = true;
    }
    if (m_pos.y < minY) {
        m_pos.y = minY;
        hitBorder = true;
    }
    if (m_pos.y > maxY) {
        m_pos.y = maxY;
        hitBorder = true;
    }

    // Nouvelle direction aléatoire si on touche le bord
    if (hitBorder) {
        m_dir = XDino_RandomUnitVec2();
    }

}

void DinoAnimal::Draw(double timeSinceStart)
{
    constexpr float TIME_FADE_IN = 1;
    float alpha = (timeSinceStart - m_spawnTime) / TIME_FADE_IN;
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart, alpha);
    XDino_Draw(vbufID, m_texID, {m_pos.x - 16, m_pos.y - 32});
    XDino_DestroyVertexBuffer(vbufID);
}

uint64_t DinoAnimal::GenerateVertexBuffer(double timeSinceStart, float alpha)
{
    uint8_t a = (uint8_t)(alpha * 255);
    int frameAnim = int(timeSinceStart * 2) % 4;
    uint16_t uMin = m_animalType * 128 + frameAnim * 32;
    uint16_t uMax = uMin + 32;
    uint16_t vMin = 0;

    if (fabs(m_dir.x) > fabs(m_dir.y)) {
        if (m_dir.x < 0) {
            vMin = 0;
        }
        else {
            vMin = 0;
            uint16_t tmp = uMin;
            uMin = uMax;
            uMax = tmp;
        }
    }
    else {
        if (m_dir.y > 0) {
            vMin = 32;
        }
        else {
            vMin = 64;
        }

    }

    uint16_t vMax = vMin + 32;

    // Dessin de la texture centrale qu'on peut bouger.
    {
        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[1].pos = {32, 0};
        vs[2].pos = {0, 32};
        vs[3].pos = {32, 0};
        vs[4].pos = {0, 32};
        vs[5].pos = {32, 32};
        vs[0].u = uMin;
        vs[0].v = vMin;
        vs[1].u = uMax;
        vs[1].v = vMin;
        vs[2].u = uMin;
        vs[2].v = vMax;
        vs[3].u = uMax;
        vs[3].v = vMin;
        vs[4].u = uMin;
        vs[4].v = vMax;
        vs[5].u = uMax;
        vs[5].v = vMax;
        vs[0].color = {{0xFF, 0xFF, 0xFF, a}};
        vs[1].color = {{0xFF, 0xFF, 0xFF, a}};
        vs[2].color = {{0xFF, 0xFF, 0xFF, a}};
        vs[3].color = {{0xFF, 0xFF, 0xFF, a}};
        vs[4].color = {{0xFF, 0xFF, 0xFF, a}};
        vs[5].color = {{0xFF, 0xFF, 0xFF, a}};

        return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");

    }
};

#pragma endregion