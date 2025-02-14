#include "dino_draw_utils.h"
#include "dino_geometry.h"

#include <dino/xdino.h>
#include <dino/dino_player.h>

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.
constexpr DinoVec2 SCREEN_SIZE = {480, 360};


void DinoPlayer::Init(DinoVec2 initm_pos, int32_t idxPlayer, DinoGamepadIdx idxGamepad, DinoColor lassoColor)
{
    m_pos = initm_pos;
    m_idxPlayer = idxPlayer;
    m_idxGamepad = idxGamepad;
    m_lassoColor = lassoColor;
}

void DinoPlayer::Update(float deltaTime)
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

    this->m_pos.x += gamepad.stick_left_x * speed * deltaTime;

    this->m_pos.y += gamepad.stick_left_y * speed * deltaTime;

    if (gamepad.stick_left_x != 0)
        this->bMirror = gamepad.stick_left_x < 0;

    this->bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!this->bIdle) {
        if (gamepad.btn_right)
            this->bRunning = true;
        else
            this->bWalking = true;
    }
    m_lasso.emplace_back(m_pos);
    if (m_lasso.size() > 120) {
        m_lasso.erase(m_lasso.begin());
    }

     
    // Détecter les collisions des segments du lasso
    // m_lasso.size() points
    // m_lasso.size() - 1 segments
    if (m_lasso.size()  >= 4) {
        DinoVec2 C = m_lasso[m_lasso.size() - 2];
        DinoVec2 D = m_lasso[m_lasso.size() - 1];
        for (int idxSegment1 = 0; idxSegment1 < m_lasso.size() - 3; ++idxSegment1) {
            DinoVec2 A = m_lasso[idxSegment1];
            DinoVec2 B = m_lasso[idxSegment1 + 1];
            
            if (Dino_IntersectSegment(A, B, C, D)) {
                // Collision du lasso avec lui-même
                m_lasso.erase(m_lasso.begin() + idxSegment1 + 1, m_lasso.end() - 1);
            }
        }
    }
    
}

void DinoPlayer::DrawLasso()
{
    // Lasso
    DinoDrawCall drawCallLasso = Dino_CreateDrawCall_Polyline(m_lasso, 4, m_lassoColor);
    XDino_Draw(drawCallLasso);
}


void DinoPlayer::Draw(double timeSinceStart)
{

    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    DinoVec2 m_posA = {-12, -22};
    DinoVec2 m_posB = {12, -22};
    DinoVec2 m_posC = {-12, 2};
    DinoVec2 m_posD = {12, 2};

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
        drawCall.vertices.emplace_back(m_posA, u + 24, v + 0);
        drawCall.vertices.emplace_back(m_posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(m_posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(m_posB, u + 0, v + 0);
        drawCall.vertices.emplace_back(m_posC, u + 24, v + 24);
        drawCall.vertices.emplace_back(m_posD, u + 0, v + 24);
    }
    else {
        drawCall.vertices.emplace_back(m_posA, u + 0, v + 0);
        drawCall.vertices.emplace_back(m_posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(m_posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(m_posB, u + 24, v + 0);
        drawCall.vertices.emplace_back(m_posC, u + 0, v + 24);
        drawCall.vertices.emplace_back(m_posD, u + 24, v + 24);
    }

    drawCall.scale = 3;
    drawCall.translation = this->m_pos;
    XDino_Draw(drawCall);
}

bool DinoPlayer::IsAbove(DinoPlayer& other)
{
    return m_pos.y < other.m_pos.y;
}