#include "dino_draw_utils.h"

#include <dino/dino_player.h>

#include <algorithm>

void DinoPlayer::Init(DinoVec2 initPos, DinoGamepadIdx idxGamepad_, int32_t idxPlayer_, DinoColor lassoColor)
{
    dinoPos = initPos;
    idxGamepad = idxGamepad_;
    idxPlayer = idxPlayer_;
    m_lassoColor = lassoColor;
    if (idxPlayer < 0 || idxPlayer > 4)
        DINO_CRITICAL("Only players indices [0...3] are accepted");
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime)
{
    // Gestion des entrées.

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(idxGamepad, gamepad);
    if (bSuccess) {
        bDinoRunning = gamepad.btn_right;
        dinoMove.x = gamepad.stick_left_x;
        dinoMove.y = gamepad.stick_left_y;
        if (gamepad.shoulder_left) {
            bDinoDamage = true;
            dinoDamageStart = timeSinceStart;
        }
    }

    // Logique de jeu.

    if (bDinoDamage && timeSinceStart - dinoDamageStart > 3)
        bDinoDamage = false;

    float speed = 120;
    if (bDinoRunning)
        speed *= 2;
    if (bDinoDamage)
        speed = 0;

    dinoPos.x += dinoMove.x * speed * deltaTime;
    dinoPos.y += dinoMove.y * speed * deltaTime;
    if (dinoMove.x < 0)
        bDinoLeft = true;
    if (dinoMove.x > 0)
        bDinoLeft = false;

    m_lassoPoints.push_back(dinoPos);
    if (m_lassoPoints.size() > 120)
        m_lassoPoints.erase(m_lassoPoints.begin());
}

std::span<DinoVec2 const> DinoPlayer::GetLasso() const
{
    return m_lassoPoints;
}

void DinoPlayer::EraseLasso(size_t idxBegin, size_t idxEnd)
{
    if (idxBegin <= idxEnd && idxEnd <= m_lassoPoints.size())
        m_lassoPoints.erase(m_lassoPoints.begin() + idxBegin, m_lassoPoints.begin() + idxEnd);
    else
        DINO_CRITICAL("Logic error in indices in EraseLasso()");
}

void DinoPlayer::_AddDrawCall(double timeSinceStart, float deltaTime, DinoDrawCall& drawCall) const
{
    drawCall.vertices.emplace_back(DinoVec2{-12, -12}, 0, 0, DinoColor_WHITE);
    drawCall.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
    drawCall.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
    drawCall.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
    drawCall.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
    drawCall.vertices.emplace_back(DinoVec2{+12, +12}, 24, 24, DinoColor_WHITE);

    std::span<DinoVertex> vertices = {drawCall.vertices.end() - 6, drawCall.vertices.end()};

    // Appliquer la position
    for (DinoVertex& vertex : vertices) {
        vertex.pos.x += dinoPos.x;
        vertex.pos.y += dinoPos.y;
    }

    // Mode miroir si on va à gauche.
    if (bDinoLeft) {
        for (DinoVertex& vertex : vertices)
            vertex.u = 24 - vertex.u;
    }

    // Animation
    if (bDinoDamage) {
        // Animation de dégât
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 3;
        for (DinoVertex& vertex : vertices)
            vertex.u += static_cast<uint16_t>(336 + 24 * idxFrame);
    }
    else if (dinoMove.x == 0 && dinoMove.y == 0) {
        // Animation sur place
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 4;
        for (DinoVertex& vertex : vertices)
            vertex.u += static_cast<uint16_t>(24 * idxFrame);
    }
    else if (bDinoRunning) {
        // Animation de course
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 16) % 6;
        for (DinoVertex& vertex : vertices)
            vertex.u += static_cast<uint16_t>(432 + 24 * idxFrame);

    }
    else {
        // Animation de marche
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 6;
        for (DinoVertex& vertex : vertices)
            vertex.u += static_cast<uint16_t>(96 + 24 * idxFrame);
    }

    // Couleur de dinosaure
    for (DinoVertex& vertex : vertices)
        vertex.v += 24 * idxPlayer;
}

DinoDrawCall DinoPlayer::DrawCallLasso() const
{
    return Dino_CreateDrawCall_Polyline(m_lassoPoints, 4, m_lassoColor);
}

DinoDrawCall DinoPlayer::DrawCallDinos(std::span<DinoPlayer const> dinos, double timeSinceStart, float deltaTime)
{
    std::vector<DinoPlayer const*> tmpDinos;
    tmpDinos.reserve(dinos.size());
    for (DinoPlayer const& dino : dinos)
        tmpDinos.push_back(&dino);

    std::sort(tmpDinos.begin(),
              tmpDinos.end(),
              [](DinoPlayer const* a, DinoPlayer const* b) {
                  return a->dinoPos.y < b->dinoPos.y;
              });

    DinoDrawCall drawDinos;
    drawDinos.textureName = "dinosaurs.png";
    for (DinoPlayer const* pDino : tmpDinos)
        pDino->_AddDrawCall(timeSinceStart, deltaTime, drawDinos);
    return drawDinos;
}