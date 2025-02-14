#include "dino_player.h"

#include "dino_geometry.h"

void dino_player::InitDino(DinoVec2 position, int index, DinoGamepadIdx idxGamepad, DinoColor color)
{
    pos = position;
    idxPlayer = index;
    m_idxGamepad = idxGamepad;
    m_color = color;
}

void dino_player::Update(float deltaTime)
{
    // Gestion des states d'anim du joueur
    this->bIdle = false;
    this->bRunning = false;
    this->bWalking = false;

    DinoGamepad gamepad;
    bool bSuccess = XDino_GetGamepad(m_idxGamepad, gamepad);
    if (!bSuccess)
        gamepad = {};

    float speed = CIRCLE_SPEED;
    if (gamepad.btn_right)
        speed *= 2;

    this->pos.x += gamepad.stick_left_x * speed * deltaTime;
    this->pos.y += gamepad.stick_left_y * speed * deltaTime;

    // Gestion des changement de states d'anim du joueur
    if (gamepad.stick_left_x != 0)
        this->bMirror = gamepad.stick_left_x < 0;

    this->bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!this->bIdle) {
        if (gamepad.btn_right)
            this->bRunning = true;
        else
            this->bWalking = true;
    }

    lassoPoints.emplace_back(pos);
    if (lassoPoints.size() > 120)
        lassoPoints.erase(lassoPoints.begin());

    // m_lasso.size() points;
    for (int idxSegment1 = 0; idxSegment1 < lassoPoints.size() - 1; ++idxSegment1) {
        DinoVec2& a = lassoPoints[idxSegment1];
        DinoVec2& b = lassoPoints[idxSegment1 + 1];

        for (int idxSegment2 = idxSegment1 + 2; idxSegment2 < lassoPoints.size() - 1; ++idxSegment2) {
            DinoVec2& c = lassoPoints[idxSegment2];
            DinoVec2& d = lassoPoints[idxSegment2 + 1];

            if (Dino_IntersectSegment(a, b, c, d)) {
                // Collision du lasson avec lui-même
                // [AB] et [CD] collisionne, on enlève tout entre B et C, on garde juste AD.
                // B : idxSegment1 + 1
                // C : idxSegment2 --> le second segment est exclu
                lassoPoints.erase(lassoPoints.begin() + idxSegment1 + 1, lassoPoints.begin() + idxSegment2 + 1);
            }
        }
    }
}

void dino_player::Draw(double timeSinceStart)
{
    DinoDrawCall drawCall;
    drawCall.textureName = "dinosaurs.png";
    DinoVec2 posA = {-12, -22};
    DinoVec2 posB = {12, -22};
    DinoVec2 posC = {-12, 2};
    DinoVec2 posD = {12, 2};

    int u = 0;
    if (bIdle) {
        int idxFrame = static_cast<int>(timeSinceStart * 8) % 4;
        u = 0 + 24 * idxFrame;
    }

    if (bWalking) {
        int idxFrame = static_cast<int>(timeSinceStart * 8) % 6;
        u = 96 + 24 * idxFrame;
    }

    if (bRunning) {
        int idxFrame = static_cast<int>(timeSinceStart * 16) % 6;
        u = 432 + 24 * idxFrame;
    }

    int v = idxPlayer * 24;

    if (bMirror) {
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
    drawCall.translation = pos;
    XDino_Draw(drawCall);
}

bool dino_player::IsAbove(dino_player& other)
{
    return pos.y < other.pos.y;
}

void dino_player::DrawLasso()
{
    // Déssine le lasso
    DinoDrawCall drawCallLasso = Dino_CreateDrawCall_Polyline(lassoPoints, 4, m_color);
    XDino_Draw(drawCallLasso);
}