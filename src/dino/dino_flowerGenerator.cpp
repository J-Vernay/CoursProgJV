#include "dino_flowerGenerator.h"
#include "xdino.h"

void Init(){

    std::vector<DinoVertex> vs;
    vs.resize(6);

    
    m_vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Terrain");
    m_texID = XDino_CreateGpuTexture("terrain.png");
}

void Draw()
{
    XDino_Draw(m_vbufID, m_texID);
}

void Shut()
{
    XDino_DestroyVertexBuffer(m_vbufID);
    XDino_DestroyGpuTexture(m_texID);
}
