#include "dino_geometry.h"

#include <algorithm>
#include <dino/xdino.h>
#include <dino/dino_player.h>

uint64_t DinoPlayer::s_texID = 0;


DinoPlayer::DinoPlayer(int idxPlayer)
{
    DinoVec2 windowSize = XDino_GetWindowSize();

    m_pos = {-150 + windowSize.x / 2 + idxPlayer * 50, windowSize.y / 2};
    m_idxPlayer = idxPlayer;

    {
        m_anims.push_back({Idle, 0, 4, 8});
        m_anims.push_back({Walk, 96, 6, 8});
        m_anims.push_back({Hit, 336, 3, 8});
        m_anims.push_back({Run, 432, 6, 16});
    }
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime, DinoTerrain terrain, DinoGamepad gamepad)
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
        DinoVec2 stick = {gamepad.stick_left_x, gamepad.stick_left_y};
        m_pos = m_pos + stick * speed * deltaTime;
        //m_pos.x += gamepad.stick_left_x * speed * deltaTime;
        //m_pos.y += gamepad.stick_left_y * speed * deltaTime;

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

    if (timeSinceStart < m_endHitAnim) {
        m_currentAnim = Hit;
    }

    DinoVec2 topLeft = terrain.GetTopLeft();
    m_pos.x = std::clamp(m_pos.x, topLeft.x + 12, topLeft.x + TERRAIN_SIZE.x - 12);
    m_pos.y = std::clamp(m_pos.y, topLeft.y + 12, topLeft.y + TERRAIN_SIZE.y - 12);

}

DinoVertexBuffer DinoPlayer::GenerateVertexBuffer(double timeSinceStart)
{
    anim current = Get_Current_Anim();
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

        return {vs.data(), vs.size(), "ImageDino"};

    }
};

anim DinoPlayer::Get_Current_Anim()
{
    for (auto a : m_anims) {
        if (a.state == m_currentAnim)
            return a;
    }

    return m_anims[0];
}

void DinoPlayer::Draw(double timeSinceStart)
{
    DinoVertexBuffer vertex_buffer = GenerateVertexBuffer(timeSinceStart);
    XDino_Draw(vertex_buffer.GetVbufID(), s_texID, {m_pos.x - 12, m_pos.y - 20});
}

void DinoPlayer::ReactLoop(double timeSinceStart, int lassoIndex)
{
    if (lassoIndex != m_idxPlayer)
        m_endHitAnim = timeSinceStart + 3;
}


void DinoPlayer::Shut()
{
}

void DinoPlayer::InitStatic()
{
    s_texID = XDino_CreateGpuTexture("dinosaurs.png");
}

void DinoPlayer::ShutStatic()
{
    XDino_DestroyGpuTexture(s_texID);
}