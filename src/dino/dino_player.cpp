#include <dino/dino_player.h>

#include <dino/xdino.h>

void DinoPlayer::Init(int idxPlayer)
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    m_pos = {windowSize.x / 2, windowSize.y / 2};
    m_texID = XDino_CreateGpuTexture("dinosaurs.png");
    m_idxPlayer = idxPlayer;
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad)
{
    m_bPressedRun = false;
    m_bMoving = false;

    constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

    float speed = DINO_SPEED;
    if (gamepad.btn_right) {
        speed = DINO_SPEED * 2;
        m_bPressedRun = true;
    }
    if (gamepad.stick_left_x != 0 || gamepad.stick_left_y != 0)
        m_bMoving = true;

    if (timeSinceStart >= m_endHitAnim) {
        m_pos.x += gamepad.stick_left_x * speed * deltaTime;
        m_pos.y += gamepad.stick_left_y * speed * deltaTime;
    }

    if (gamepad.stick_left_x < 0)
        m_bLeft = true;
    if (gamepad.stick_left_x > 0)
        m_bLeft = false;

    if (gamepad.btn_left) {
        m_endHitAnim = timeSinceStart + 3;
    }
}

void DinoPlayer::Draw(double timeSinceStart)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart);
    XDino_Draw(vbufID, m_texID, m_pos, 4);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoPlayer::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}

void DinoPlayer::Draw(double timeSinceStart)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart);
    XDino_Draw(vbufID, m_texID, m_pos, 4);
    XDino_DestroyVertexBuffer(vbufID);
}

uint64_t DinoPlayer::GenerateVertexBuffer(double timeSinceStart)
{
    float animSpeed;
    int frameCount;
    int ubase;
    if (timeSinceStart < m_endHitAnim) {
        // ANIM HIT
        animSpeed = 8;
        frameCount = 3;
        ubase = 336;
    }
    else if (m_bMoving) {
        if (m_bPressedRun) {
            // ANIM RUN
            animSpeed = 16;
            frameCount = 6;
            ubase = 432;
        }
        else {
            // ANIM WALK
            animSpeed = 8;
            frameCount = 6;
            ubase = 96;
        }
    }
    else {
        // ANIM IDLE
        animSpeed = 8;
        frameCount = 4;
        ubase = 0;
    }

    int uAnim = ((int)(timeSinceStart * animSpeed) % frameCount) * 24 + ubase;

    std::vector<DinoVertex> vs;
    uint16_t umin, umax;
    if (m_bLeft) {
        umin = uAnim + 24;
        umax = uAnim + 0;
    }
    else {
        umin = uAnim + 0;
        umax = uAnim + 24;
    }

    uint16_t vbase = 24 * m_idxPlayer;

    vs.resize(6);
    vs[0].pos = {0, 0};
    vs[0].u = umin;
    vs[0].v = vbase;
    vs[1].pos = {24, 0};
    vs[1].u = umax;
    vs[1].v = vbase;
    vs[2].pos = {0, 24};
    vs[2].u = umin;
    vs[2].v = vbase + 24;
    vs[3].pos = {24, 0};
    vs[3].u = umax;
    vs[3].v = vbase;
    vs[4].pos = {0, 24};
    vs[4].u = umin;
    vs[4].v = vbase + 24;
    vs[5].pos = {24, 24};
    vs[5].u = umax;
    vs[5].v = vbase + 24;
    return XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
}