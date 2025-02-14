#include "dino_player.h"

#include "dino_draw_utils.h"
#include "dino_geometry.h"
#include "dino/xdino.h"


void DinoPlayer::Update(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.

    this->isIdle = false;
    this->isWalking = false;
    this->isRunning = false;

    DinoGamepad gamepad;
    bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
    if (!bSuccess)
        gamepad = {}; // Laisser vide, on considère le joueur immobile

    float speed = baseSpeed;
    if (gamepad.btn_right)
        speed *= 2;

    this->playerPos.x += gamepad.stick_left_x * speed * deltaTime;
    this->playerPos.y += gamepad.stick_left_y * speed * deltaTime;

    if (gamepad.stick_left_x != 0)
        this->g_bMiror = gamepad.stick_left_x < 0;

    this->isIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!this->isIdle) {
        if (gamepad.btn_right)
            this->isRunning = true;
        else
            this->isWalking = true;
    }

    m_lasso.emplace_back(playerPos);
    if (m_lasso.size() > 120) {
        m_lasso.erase(m_lasso.begin());
    }

    // Détecter les collisions des segments du lasso

    // m_lasso.size() points
    // m_lasso.size() - 1 segments

    for (int idxSegment1 = 0; idxSegment1 < m_lasso.size() - 1; ++idxSegment1) {
        DinoVec2 A = m_lasso[idxSegment1];
        DinoVec2 B = m_lasso[idxSegment1 + 1];

        for (int idxSegment2 = idxSegment1 + 2; idxSegment2 < m_lasso.size() - 1; ++idxSegment2) {
            DinoVec2 C = m_lasso[idxSegment2];
            DinoVec2 D = m_lasso[idxSegment2 + 1];

            if (Dino_IntersectSegment(A, B, C, D)) {
                // Collision du lasso avec lui-même
                // [AB] et [CD] collisionne, on enlève tout entre B et C, on garde juste AD.
                // B : idxSegment1 + 1
                // C : idxSegment2 --> le second argument de erase() est EXCLU de l'intervalle, donc on doit faire + 1
                m_lasso.erase(m_lasso.begin() + idxSegment1 + 1, m_lasso.begin() + idxSegment2 + 1);
            }
        }
    }
}

void DinoPlayer::DrawLasso()
{
    // Lasso
    DinoDrawCall drawCallLasso = Dino_CreateDrawCall_Polyline(m_lasso, 4, color);
    XDino_Draw(drawCallLasso);
}


void DinoPlayer::Draw(double time)
{
    DinoDrawCall drawcall;
    drawcall.textureName = "greyscale_Dino.png";

    DinoVec2 posA = {-12, -22};
    DinoVec2 posB = {12, -22};
    DinoVec2 posC = {-12, 2};
    DinoVec2 posD = {12, 2};

    int u = 0;
    if (this->isIdle) {
        int idxFrame = int(time * 8) % 4;
        u = 0 + 24 * idxFrame;
    }
    if (this->isWalking) {
        int idxFrame = int(time * 8) % 6;
        u = 96 + 24 * idxFrame;
    }
    if (this->isRunning) {
        int idxFrame = int(time * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    if (this->g_bMiror) {
        drawcall.vertices.emplace_back(posA, 24 + u, 0);
        drawcall.vertices.emplace_back(posB, 0 + u, 0);
        drawcall.vertices.emplace_back(posC, 24 + u, 24);
        drawcall.vertices.emplace_back(posB, 0 + u, 0);
        drawcall.vertices.emplace_back(posC, 24 + u, 24);
        drawcall.vertices.emplace_back(posD, 0 + u, 24);
    }
    else {
        drawcall.vertices.emplace_back(posA, 0 + u, 0);
        drawcall.vertices.emplace_back(posB, 24 + u, 0);
        drawcall.vertices.emplace_back(posC, 0 + u, 24);
        drawcall.vertices.emplace_back(posB, 24 + u, 0);
        drawcall.vertices.emplace_back(posC, 0 + u, 24);
        drawcall.vertices.emplace_back(posD, 24 + u, 24);
    }
    drawcall.translation = this->playerPos;
    drawcall.scale = 1;
    for (DinoVertex& vertice : drawcall.vertices) {
        vertice.color = color;

    }
    XDino_Draw(drawcall);
}

void DinoPlayer::Init(DinoVec2 pos, DinoGamepadIdx idx, int indexGraph = 0)
{
    playerPos = pos;
    gamepadIdx = idx;
    color = indexGraph == 0
                ? DinoColor_RED
                : indexGraph == 1
                ? DinoColor_BLUE
                : indexGraph == 2
                ? DinoColor_YELLOW
                : DinoColor_GREEN;
}