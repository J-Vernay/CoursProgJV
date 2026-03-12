#include "dino_draw_utils.h"

#include <algorithm>
#include <dino/xdino.h>
#include <dino/DinoController.h>
#include <dino/dino_geometry.h>

// Constantes.
constexpr float DINO_SPEED = 150.f; // Nombre de pixels parcourus en une seconde.
constexpr float DINO_RUN_SPEED = 0.5f * DINO_SPEED; // Running Speed
constexpr float DINO_SCALE = 1.f; // Dino Scale

// - All anims lenght
constexpr int ANIM_IDLE_LEN = 4;
constexpr int ANIM_WALK_LEN = 6;
constexpr int ANIM_HURT_LEN = 3;
constexpr int ANIM_RUN_LEN = 6;

constexpr int DINO_ANIM_FRAMES_PER_SECOND = 8;

// - All Corners of Dino Sprite
constexpr DinoVec2 UPPER_LEFT = {0, 0};
constexpr DinoVec2 UPPER_RIGHT = {24, 0};
constexpr DinoVec2 LOWER_LEFT = {0, 24};
constexpr DinoVec2 LOWER_RIGHT = {24, 24};

uint64_t DinoControllerFields::s_texID = 0;

void DinoControllerFields::Init(int playerCount)
{
    playerCount;

    DinoVec2 windowSize = {480, 360};
    this->m_pos = {windowSize.x / 2, windowSize.y / 2};
    this->m_dinoColor = playerCount;

    // if (playerCount == 0)
    //     m_lassoColor = DinoColor{77, 146, 188, 255}; // BLUE
    // else if (playerCount == 1)
    //     m_lassoColor = DinoColor{188, 77, 79, 255}; // RED
    // else if (playerCount == 2)
    //     m_lassoColor = DinoColor{253, 199, 96, 255}; // YELLOW
    // else if (playerCount == 3)
    //     m_lassoColor = DinoColor{159, 188, 77, 255}; // GREEN
}

// void DinoControllerFields::Shut()
// {
//     
// }

void DinoControllerFields::DinoMovement(DinoGamepad gamepad, float deltaTime)
{
    m_gamepad = gamepad;

    if (this->m_dinoTakeDamage)
        return;

    // Prevent Diagonal speed boost
    if (m_gamepad.stick_left_x != 0.0f && m_gamepad.stick_left_y != 0.0f)
        this->m_dinoCurrentSpeed = 0.75f * DINO_SPEED;
    else
        this->m_dinoCurrentSpeed = DINO_SPEED;

    DinoVec2 dinoPosDelta = {0, 0};
    dinoPosDelta.x += m_gamepad.stick_left_x * (this->m_dinoCurrentSpeed + m_gamepad.btn_right * DINO_RUN_SPEED) *
        deltaTime;
    dinoPosDelta.y += m_gamepad.stick_left_y * (this->m_dinoCurrentSpeed + m_gamepad.btn_right * DINO_RUN_SPEED) *
        deltaTime;

    m_pos.x += dinoPosDelta.x;
    m_pos.y += dinoPosDelta.y;

    // m_lasso.push_back(m_pos);
    //
    // if (m_lasso.size() >= 120) {
    //     m_lasso.erase(m_lasso.begin());
    // }
    //
    // CheckForOwnLassoIntersections();
}

void DinoControllerFields::ReactLimit(bool xChanged)
{
}

void DinoControllerFields::ReactLoop(double timeSinceStart)
{
    if (!m_dinoTakeDamage) {
        this->m_dinoDamageAnimTimer = timeSinceStart + ANIM_HURT_LEN;
        m_dinoTakeDamage = true;
    }
}

uint64_t DinoControllerFields::GenDinoVertexBuffer(float timeSinceStart)
{
    // Choosing Animation based on current player behaviour
    int currAnimLen;
    int animationSpeed;
    int firstFrameOfAnim;

    // Temp condition to test damage
    if (this->m_dinoTakeDamage) {
        currAnimLen = ANIM_HURT_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + 4;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;

        if (this->m_dinoDamageAnimTimer <= timeSinceStart) {
            this->m_dinoTakeDamage = false;
        }
    }
    else if (m_gamepad.stick_left_x == 0.0f && m_gamepad.stick_left_y == 0.0f) {
        currAnimLen = ANIM_IDLE_LEN;
        firstFrameOfAnim = 0;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    }
    else if (m_gamepad.btn_right == 0.0f) {
        currAnimLen = ANIM_WALK_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    }
    else {
        currAnimLen = ANIM_RUN_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + ANIM_HURT_LEN + 4 + 1;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND * 2;
    }

    if (this->m_dinoAnimElapsed > 1.f / animationSpeed) {
        this->m_currFrame = (this->m_currFrame + 1) % currAnimLen;
        this->m_dinoAnimElapsed = 0.0f;
    }
    else {
        this->m_dinoAnimElapsed += timeSinceStart;
    }

    // Displaying Dino

    // Only change facing orientation if player is moving
    if (m_gamepad.stick_left_x != 0.0f) {
        this->m_dinoGoingLeft = m_gamepad.stick_left_x <= 0.0f; // Changes facing orientation based on input
    }

    std::vector<DinoVertex> vs;
    vs.resize(6);

    vs[0].pos = this->m_dinoGoingLeft ? UPPER_RIGHT : UPPER_LEFT;
    vs[0].u = 0 + (firstFrameOfAnim + this->m_currFrame) * 24;
    vs[0].v = 0 + this->m_dinoColor * 24;

    vs[1].pos = this->m_dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
    vs[1].u = 24 + (firstFrameOfAnim + this->m_currFrame) * 24;
    vs[1].v = 0 + this->m_dinoColor * 24;

    vs[2].pos = this->m_dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
    vs[2].u = 0 + (firstFrameOfAnim + this->m_currFrame) * 24;
    vs[2].v = 24 + this->m_dinoColor * 24;

    vs[3].pos = this->m_dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
    vs[3].u = 24 + (firstFrameOfAnim + this->m_currFrame) * 24;
    vs[3].v = 0 + this->m_dinoColor * 24;

    vs[4].pos = this->m_dinoGoingLeft ? LOWER_LEFT : LOWER_RIGHT;
    vs[4].u = 24 + (firstFrameOfAnim + this->m_currFrame) * 24;
    vs[4].v = 24 + this->m_dinoColor * 24;

    vs[5].pos = this->m_dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
    vs[5].u = 0 + (firstFrameOfAnim + this->m_currFrame) * 24;
    vs[5].v = 24 + this->m_dinoColor * 24;

    this->vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
    return this->vbufID_dino;
}

void DinoControllerFields::Draw(double timeSinceStart)
{
    this->vbufID_dino = GenDinoVertexBuffer(timeSinceStart);
    // -12 / -18 to move the position to the feet and not the corner of the texture
    XDino_Draw(this->vbufID_dino, s_texID, {this->m_pos.x - 12, this->m_pos.y - 18}, DINO_SCALE);
    XDino_DestroyVertexBuffer(this->vbufID_dino);
}

// void DinoControllerFields::DrawLasso()
// {
//     std::vector<DinoVertex> vs;
//     Dino_GenVertices_Polyline(vs, m_lasso, 5, m_lassoColor);
//     vbufID_lasso = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Polyline");
//     XDino_Draw(vbufID_lasso, XDino_TEXID_WHITE);
//     XDino_DestroyVertexBuffer(vbufID_lasso);
// }
//
// void DinoControllerFields::CheckForOwnLassoIntersections()
// {
//     if (m_lasso.size() < 4)
//         return;
//
//     DinoVec2 c = m_lasso[m_lasso.size() - 2];
//     DinoVec2 d = m_lasso[m_lasso.size() - 1];
//
//     if (c.x == d.x && c.y == d.y)
//         return;
//
//     for (int i = 0; i < m_lasso.size() - 3; i++) {
//         DinoVec2 a = m_lasso[i];
//         DinoVec2 b = m_lasso[i + 1];
//         if (Dino_IntersectSegment(a, b, c, d)) {
//             m_lasso.erase(m_lasso.begin() + i + 1, m_lasso.end() - 1);
//             m_lassoLifeSpan = 2.f;
//             break;
//         }
//     }
// }
//
// void DinoControllerFields::CheckForOtherLassIntersections(DinoControllerFields& other)
// {
//     if (m_lasso.size() < 2 || other.m_lasso.size() < 2)
//         return;
//
//     DinoVec2 a = m_lasso[m_lasso.size() - 2];
//     DinoVec2 b = m_lasso[m_lasso.size() - 1];
//
//     if (a.x == b.x && a.y == b.y)
//         return;
//
//     for (int i = 0; i < other.m_lasso.size() - 2; i++) {
//         DinoVec2 c = other.m_lasso[i];
//         DinoVec2 d = other.m_lasso[i + 1];
//
//         if (Dino_IntersectSegment(a, b, c, d)) {
//             other.m_lasso.erase(other.m_lasso.begin(), other.m_lasso.begin() + i + 1);
//             other.m_lassoLifeSpan = 2.f;
//             break;
//         }
//     }
// }

void DinoControllerFields::InitTexture()
{
    s_texID = XDino_CreateGpuTexture("dinosaurs.png");
}

void DinoControllerFields::ShutTexture()
{
    XDino_DestroyGpuTexture(s_texID);
}