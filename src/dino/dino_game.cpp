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
float stepAnim = 24;

float animPos1 = 0;
float animPos2 = 24;

float t = 0;
float animTick = 0.05f;


constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

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
        std::string text = std::format("BOULANGER Antoine");
        std::vector<DinoVertex> vs2;
        textSize = Dino_GenVertices_Text(vs2, text, DinoColor_WHITE, DinoColor_GREY);
        vbufID_Antoine = XDino_CreateVertexBuffer(vs2.data(), vs2.size(), "Antoine");
    }

}

void setDinoSprite()
{
    {
        texID_imageDino = XDino_CreateGpuTexture("dinosaurs.png");
        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {0, 0};
        vs[0].u = isLookingRight ? animPos1 : animPos2;
        vs[0].v = 0;

        vs[1].pos = {24, 0};
        vs[1].u = isLookingRight ? animPos2 : animPos1;
        vs[1].v = 0;

        vs[2].pos = {0, 24};
        vs[2].u = isLookingRight ? animPos1 : animPos2;
        vs[2].v = 24;

        vs[3].pos = {24, 0};
        vs[3].u = isLookingRight ? animPos2 : animPos1;
        vs[3].v = 0;

        vs[4].pos = {0, 24};
        vs[4].u = isLookingRight ? animPos1 : animPos2;
        vs[4].v = 24;

        vs[5].pos = {24, 24};
        vs[5].u = isLookingRight ? animPos2 : animPos1;
        vs[5].v = 24;

        vbufID_imageDino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageDino");
        XDino_Draw(vbufID_imageDino, texID_imageDino, g_circlePos, 4);
        XDino_DestroyGpuTexture(texID_imageDino);
    }
}

void animHandleDino(int step, int start, int maxSetp1, int maxSetp2)
{
    int startanimPos = start;
    int startanimPos2 = start + 24;

    if (t >= animTick) {
        animPos1 += step;
        animPos2 += step;
        if (animPos1 > maxSetp1)animPos1 = startanimPos;
        if (animPos2 > maxSetp2)animPos2 = startanimPos2;
        t = 0;
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

        if (gamepad.select) {
            g_circlePos.x += gamepad.stick_left_x * CIRCLE_SPEED * 2 * deltaTime;
            g_circlePos.y += gamepad.stick_left_y * CIRCLE_SPEED * 2 * deltaTime;
        }
        else {
            g_circlePos.x += gamepad.stick_left_x * CIRCLE_SPEED * deltaTime;
            g_circlePos.y += gamepad.stick_left_y * CIRCLE_SPEED * deltaTime;
        }

        if (gamepad.stick_left_x > 0) {
            isLookingRight = true;
        }
        else if (gamepad.stick_left_x < 0) {
            isLookingRight = false;
            
        }

        if (gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0) {
           // animHandleDino(24, 0, 48, 72);
        }
        else {
            animHandleDino(24, 96, 192, 216);
        }

    }
    t += deltaTime;

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);
    XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    DinoVec2 renderSize = XDino_GetRenderSize();

    setDinoSprite();
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