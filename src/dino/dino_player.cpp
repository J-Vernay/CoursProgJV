#include <dino/xdino.h>
#include <dino/dino_player.h>

void DinoPlayer::Init(int idxPlayer)
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    m_pos = {-150 + windowSize.x / 2 + idxPlayer * 50, windowSize.y / 2};
    m_texID = XDino_CreateGpuTexture("dinosaurs.png");
    m_idxPlayer = idxPlayer;
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime, DinoGamepad gamepad)
{
    constexpr float Dino_SPEED = 100.f;

    float speed = Dino_SPEED;
    if (gamepad.btn_right) {
        speed *= 2;
        m_currentAnim = Run;
    }
    else {
        m_currentAnim = Walk;
    }

    if (timeSinceStart >= m_endHitAnim) {
        m_pos.x += gamepad.stick_left_x * speed * deltaTime;
        m_pos.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x < 0) {
            m_bRight = false;
        }
        if (gamepad.stick_left_x > 0) {
            m_bRight = true;
        }
    }

    if (!(gamepad.stick_left_x < 0 || gamepad.stick_left_x > 0 || gamepad.stick_left_y < 0 || gamepad.stick_left_y
          > 0)) {
        m_currentAnim = Idle;
    }

    if (gamepad.btn_left) {
        m_endHitAnim = timeSinceStart + 3;
    }

    if (timeSinceStart < m_endHitAnim) {
        m_currentAnim = Hit;
    }
}

uint64_t DinoPlayer::GenerateVertexBuffer(double timeSinceStart, anim current)
{

    int frame = int(timeSinceStart * current.Speed) % int(current.NbFrames);

    float U = current.Ubase + frame * 24;

    uint16_t vbase = 24 * m_idxPlayer;
    // Dessin de la texture centrale qu'on peut bouger.
    {
        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[1].pos = {24, 0};
        vs[2].pos = {0, 24};
        vs[3].pos = {24, -0};
        vs[4].pos = {0, 24};
        vs[5].pos = {24, 24};
        vs[0].u = m_bRight ? U : U + 24;
        vs[0].v = vbase;
        vs[1].u = m_bRight ? U + 24 : U;
        vs[1].v = vbase;
        vs[2].u = m_bRight ? U : U + 24;
        vs[2].v = vbase + 24;;
        vs[3].u = m_bRight ? U + 24 : U;
        vs[3].v = vbase;
        vs[4].u = m_bRight ? U : U + 24;
        vs[4].v = vbase + 24;;
        vs[5].u = m_bRight ? U + 24 : U;
        vs[5].v = vbase + 24;;

        return XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageDino");

    }
};

void DinoPlayer::Draw(double timeSinceStart, anim currentAnim)
{
    uint64_t vbufID = GenerateVertexBuffer(timeSinceStart, currentAnim);
    XDino_Draw(vbufID, m_texID, m_pos);
    XDino_DestroyVertexBuffer(vbufID);
}

void DinoPlayer::Shut()
{
    XDino_DestroyGpuTexture(m_texID);
}