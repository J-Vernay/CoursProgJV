#include "dino_animaux.h"
#include "dino/xdino.h"

void DinoAnimaux::Init(int idxAnimaux)
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    // avoir une postion random dans l'espace de l'écran
    // text id prendre un animaux aléatoire en fonction de la texture
    m_idxAnimaux = idxAnimaux;
}

uint64_t DinoAnimaux::GenerateVertexBuffer(double timeSinceStart)
{
    float animSpeed;
    int frameCount;
    int vbase;

    if (m_bUp) {
        vbase = 64;
    }
    else if (m_bDown) {
        vbase = 32;
    }
    else {
        vbase = 0;
    }

}