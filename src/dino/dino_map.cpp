#include "dino_map.h"
#include "dino_draw_utils.h"

void DinoMap::DisplayMap()
{
    XDino_Draw(Dino_CreateTerrain({256,192}));
}
