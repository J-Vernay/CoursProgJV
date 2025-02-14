#include <dino/dino_entity.h>
DinoVec2 DinoEntity::GetPos()
{
    return m_pos;
}
void DinoEntity::SetPos(DinoVec2 newPos)
{
    m_pos = newPos;
}