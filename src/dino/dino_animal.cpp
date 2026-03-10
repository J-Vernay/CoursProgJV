#include <dino/dino_animal.h>

void DinoAnimal::Init(int animalIndex)
{
    m_animalType = animalIndex;
    m_texID = XDino_CreateGpuTexture("animals.png");
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
        vs[0].pos = {200, 200};
        vs[1].pos = {232, 200};
        vs[2].pos = {200, 232};
        vs[3].pos = {232, 200};
        vs[4].pos = {200, 232};
        vs[5].pos = {232, 232};
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

void DinoAnimal::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}