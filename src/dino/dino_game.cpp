/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>


double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;
DinoVec2 g_circlePos = {};

uint64_t vbufID_polyline;
uint64_t vbufID_imageDino;
uint64_t texID_imageDino;

uint64_t vbufID_Antoine;
DinoVec2 textSize;


int g_debugScroll = 0;
bool isLookingRight = true;

enum animState {
    Idle = 0,
    Walk = 1,
    Hit = 2,
    Run = 3,
};

struct anim {
    animState state;
    float Ubase;
    float NbFrames;
    float Speed;
};

std::vector<anim> anims;
animState g_currentAnim = Idle;

constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.


void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_circlePos = {windowSize.x / 2, windowSize.y / 2};

    {
        constexpr DinoColor POLYLINE_COLOR = {70, 70, 100, 255};

        std::vector<DinoVec2> polyline;
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.25f);
        polyline.emplace_back(windowSize.x * 0.2f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.6f, windowSize.y * 0.75f);
        polyline.emplace_back(windowSize.x * 0.8f, windowSize.y * 0.50f);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Polyline(vs, polyline, 100, POLYLINE_COLOR);
        vbufID_polyline = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Polyline");
    }

    {

        texID_imageDino = XDino_CreateGpuTexture("dinosaurs.png");
        DinoVec2 texSize = XDino_GetGpuTextureSize(texID_imageDino);

        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[1].pos = {24, 0};
        vs[2].pos = {0, 24};
        vs[3].pos = {24, -0};
        vs[4].pos = {0, 24};
        vs[5].pos = {24, 24};
        vs[0].u = isLookingRight ? 0 : 24;
        vs[0].v = 0;
        vs[1].u = isLookingRight ? 24 : 0;
        vs[1].v = 0;
        vs[2].u = isLookingRight ? 0 : 24;
        vs[2].v = 24;
        vs[3].u = isLookingRight ? 24 : 0;
        vs[3].v = 0;
        vs[4].u = isLookingRight ? 0 : 24;
        vs[4].v = 24;
        vs[5].u = isLookingRight ? 24 : 0;
        vs[5].v = 24;

        vbufID_imageDino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageDino");

        anims.push_back({Idle, 0, 4, 8});
        anims.push_back({Walk, 96, 6, 8});
        anims.push_back({Hit, 336, 3, 8});
        anims.push_back({Run, 432, 6, 16});
    }

    {
        std::string text = std::format("BOULANGER Antoine");
        std::vector<DinoVertex> vs2;
        textSize = Dino_GenVertices_Text(vs2, text, DinoColor_WHITE, DinoColor_GREY);
        vbufID_Antoine = XDino_CreateVertexBuffer(vs2.data(), vs2.size(), "Antoine");
    }

}

anim GetCurrentAnim()
{
    for (auto a : anims) {
        if (a.state == g_currentAnim)
            return a;
    }

    return anims[0];
}

void setDinoSprite(float timeSinceStart)
{
    anim current = GetCurrentAnim();
    int frame = static_cast<int>(timeSinceStart * current.Speed) % static_cast<int>(current.NbFrames);
    float U = current.Ubase + frame * 24;
    {
        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[1].pos = {24, 0};
        vs[2].pos = {0, 24};
        vs[3].pos = {24, -0};
        vs[4].pos = {0, 24};
        vs[5].pos = {24, 24};
        vs[0].u = isLookingRight ? U : U + 24;
        vs[0].v = 0;
        vs[1].u = isLookingRight ? U + 24 : U;
        vs[1].v = 0;
        vs[2].u = isLookingRight ? U : U + 24;
        vs[2].v = 24;
        vs[3].u = isLookingRight ? U + 24 : U;
        vs[3].v = 0;
        vs[4].u = isLookingRight ? U : U + 24;
        vs[4].v = 24;
        vs[5].u = isLookingRight ? U + 24 : U;
        vs[5].v = 24;

        vbufID_imageDino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageDino");
        XDino_Draw(vbufID_imageDino, texID_imageDino, g_circlePos, 4);
        XDino_DestroyVertexBuffer(vbufID_imageDino);
    }
}


void Dino_GameFrame(double timeSinceStart)
{

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.btn_down && !gamepad.btn_up)
            g_scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            g_scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            g_rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            g_rotation -= 90.0 * deltaTime;

        float speed = DINO_SPEED;
        if (gamepad.select) {
            speed *= 2;
            g_currentAnim = Run;
        }
        else {
            g_currentAnim = Walk;
        }

        if (gamepad.stick_left_x > 0) {
            isLookingRight = true;
        }
        else if (gamepad.stick_left_x < 0) {
            isLookingRight = false;

        }

        g_circlePos.x += gamepad.stick_left_x * speed * deltaTime;
        g_circlePos.y += gamepad.stick_left_y * speed * deltaTime;

        if (!(gamepad.stick_left_x < 0 || gamepad.stick_left_x > 0 || gamepad.stick_left_y < 0 || gamepad.stick_left_y >
              0)) {
            g_currentAnim = Idle;
        }

    }
    
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    DinoVec2 renderSize = XDino_GetRenderSize();
    setDinoSprite(timeSinceStart);
    XDino_Draw(vbufID_Antoine, XDino_TEXID_FONT, {renderSize.x - (textSize.x * 2), renderSize.y - (textSize.y * 2)}, 2);

    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

#if !XDINO_RELEASE
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.shoulder_left) {
        int diff = 0;
        if (keyboard.dpad_up)
            diff -= 1;
        if (keyboard.dpad_down)
            diff += 1;
        XDino_DrawStats(diff);
    }
#endif
}

void Dino_GameShut()
{
    XDino_DestroyVertexBuffer(vbufID_Antoine);
    XDino_DestroyVertexBuffer(vbufID_imageDino);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyGpuTexture(texID_imageDino);
}