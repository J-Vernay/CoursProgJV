#include <dino/dino_animal.h>


constexpr DinoVec2 TILE_COUNT = {20, 16};
constexpr DinoVec2 TERRAIN_SIZE = {TILE_COUNT.x * 16, TILE_COUNT.y * 16};

#pragma region AnimalSpawner
void DinoAnimalSpawner::Init()
{
    m_texID = XDino_CreateGpuTexture("animals.png");
}

void DinoAnimalSpawner::Update(double timeSinceStart)
{
    if (timeSinceStart - m_timeSinceLastSpawn > 1) {
        int animalIdx = XDino_RandomInt32(0, 7);

        m_animals.emplace_back();
        m_animals.back().Init(animalIdx, m_texID);

        m_timeSinceLastSpawn = timeSinceStart;
    }

    for (auto& animal : m_animals) {
        animal.Update(timeSinceStart);
        animal.Draw(timeSinceStart);
    }
}

void DinoAnimalSpawner::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}
#pragma endregion

#pragma region AnimalBehaviours
void DinoAnimal::Init(int animalIndex, uint64_t texID)
{
    m_animalType = animalIndex;
    m_texID = texID;
    DinoVec2 rdrSize = XDino_GetRenderSize();
    m_posTopLeft = {
        (rdrSize.x - TERRAIN_SIZE.x) / 2,
        (rdrSize.y - TERRAIN_SIZE.y) / 2
    };
    m_pos = DinoVec2(m_posTopLeft.x + XDino_RandomInt32(1, TILE_COUNT.x - 1) * 16,
                     m_posTopLeft.y + XDino_RandomInt32(1, TILE_COUNT.y - 1) * 16
    );
    m_dir = {XDino_RandomFloat(-1, 1), XDino_RandomFloat(-1, 1)};
}

void DinoAnimal::Update(double timeSinceStart)
{
    m_pos.x += m_dir.x;
    m_pos.y += m_dir.y;

    if (m_pos.x <= m_posTopLeft.x) {
        m_dir.x = -m_dir.x;
    }
    if (m_pos.x >= m_posTopLeft.x + TERRAIN_SIZE.x - 32) {
        m_dir.x = -m_dir.x;
    }
    if (m_pos.y <= m_posTopLeft.y - 16) {
        m_dir.y = -m_dir.y;
    }
    if (m_pos.y >= m_posTopLeft.y + TERRAIN_SIZE.y - 32) {
        m_dir.y = -m_dir.y;
    }
}

void DinoAnimal::Draw(double timeSinceStart)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart);
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

uint64_t DinoAnimal::GenerateVertexBuffer(double timeSinceStart)
{
    int frameAnim = int(timeSinceStart * 2) % 4;

    uint16_t ubase = 128 * m_animalType + frameAnim * 32;
    uint16_t vbase = 0;
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
        vs[0].u = ubase;
        vs[0].v = vbase;
        vs[1].u = ubase + 32;
        vs[1].v = vbase;
        vs[2].u = ubase;
        vs[2].v = vbase + 32;
        vs[3].u = ubase + 32;
        vs[3].v = vbase;
        vs[4].u = ubase;
        vs[4].v = vbase + 32;
        vs[5].u = ubase + 32;
        vs[5].v = vbase + 32;

        return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");

    }
};

#pragma endregion