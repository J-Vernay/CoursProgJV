#include "dino_terrain.h"

void DinoTerrain::Init()
{
    texID_terrain = XDino_CreateGpuTexture("terrain.png");

    season = 2;

}

void DinoTerrain::Draw(double timeSinceStart)
{
    DrawOcean();
    DrawGround();
    DrawFlowers();
}

void DinoTerrain::DrawOcean() const
{

}

void DinoTerrain::DrawGround() const
{

}

void DinoTerrain::DrawFlowers() const
{

}

void DinoTerrain::Shut()
{
    XDino_DestroyGpuTexture(texID_terrain);
}