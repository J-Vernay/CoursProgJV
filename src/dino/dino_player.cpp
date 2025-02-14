
#include <dino/xdino.h>
#include <dino/dino_player.h>

// Constantes.
constexpr float CIRCLE_SPEED = 100.f; // Nombre de pixels parcourus en une seconde.

void DinoPlayer::Init(DinoVec2 initPos, int32_t idxPlayer, DinoGamepadIdx idxGamepad)
{
    pos = initPos;
    m_idxPlayer = idxPlayer;
    m_idxGamepad = idxGamepad;
}

void DinoPlayer::UpdatePlayer(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.
    
    this->bIdle = false;
    this->bWalking = false;
    this->bRunning = false;
    
    DinoGamepad gamepad;
    bool bSuccess = XDino_GetGamepad(m_idxGamepad, gamepad);
    if (!bSuccess)
        gamepad = {}; // Laisser vide, on considère le joueur immobile

    float speed = CIRCLE_SPEED;
    if (gamepad.btn_right)
        speed *= 2;
    
    this->pos.x += gamepad.stick_left_x * speed * deltaTime;
    this->pos.y += gamepad.stick_left_y * speed * deltaTime;

    if (gamepad.stick_left_x != 0)
        this->bMirror = gamepad.stick_left_x < 0;

    this->bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!this->bIdle) {
        if (gamepad.btn_right)
            this->bRunning = true;
        else
            this->bWalking = true;
    }
}

void DinoPlayer::DrawPlayer(double timeSinceStart)
{
    // Copier-coller de l'ancien code.
    
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    DinoVec2 posA = {-12, -22};
    DinoVec2 posB = {12, -22};
    DinoVec2 posC = {-12, 2};
    DinoVec2 posD = {12, 2};

    int u = 0;
    if (this->bIdle) {
        int idxFrame = int(timeSinceStart * 8) % 4;
        u = 0 + 24 * idxFrame;
    }
    if (this->bWalking) {
        int idxFrame = int(timeSinceStart * 8) % 6;
        u = 96 + 24 * idxFrame;
    }
    if (this->bRunning) {
        int idxFrame = int(timeSinceStart * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    int v = m_idxPlayer * 24; // 24 pixels par ligne de sprite

    if (this->bMirror) {
        drawCall.vertices.emplace_back(posA, u + 24, v + 0);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(posD, u + 0, v + 24);
    }
    else {
        drawCall.vertices.emplace_back(posA, u + 0, v + 0);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(posD, u + 24, v + 24);
    }
    
    drawCall.scale = 1;
    drawCall.translation = this->pos;
    XDino_Draw(drawCall);
}

bool DinoPlayer::IsAbove(DinoPlayer& other)
{
    return pos.y < other.pos.y;
}

void DinoPlayer::ApplyTerrain(DinoVec2 a, DinoVec2 b)
{
    if (pos.y < a.y)
        pos.y = a.y;
    if (pos.y > b.y)
        pos.y = b.y;
    if (pos.x < a.x)
        pos.x = a.x;
    if (pos.x > b.x)
        pos.x = b.x;
}
