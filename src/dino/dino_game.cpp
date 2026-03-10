/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <map>

#include <format>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t texID_dino;

struct DinoControllerFields {
    uint64_t vbufID_dino;

    DinoVec2 dinoPos;
    double dinoCurrentSpeed;
    bool dinoGoingLeft = false;

    float dinoAnimElapsed = 0;
    float dinoDamageAnimTimer;
    int currFrame = 0;
    int dinoColor = 0;

    bool g_dinoCanTakeDamage = true;
};

std::map<DinoGamepadIdx, DinoControllerFields> GamepadControllers;

//uint64_t vbufID_imageMilieu;
//uint64_t texID_imageMilieu;
//uint64_t vbufID_circle;
uint64_t vbufID_polyline;

uint64_t vbufID_prenom;
DinoVec2 textSize_Prenom;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.
constexpr float DINO_RUN_SPEED = 0.5f * DINO_SPEED; // Running Speed
constexpr float DINO_SCALE = 2.f; // Dino Scale
constexpr int DINO_ANIM_FRAMES_PER_SECOND = 8;

// - All Corners of Dino Sprite
constexpr DinoVec2 UPPER_LEFT = {0, 0};
constexpr DinoVec2 UPPER_RIGHT = {24, 0};
constexpr DinoVec2 LOWER_LEFT = {0, 24};
constexpr DinoVec2 LOWER_RIGHT = {24, 24};

// - All anims lenght
constexpr int ANIM_IDLE_LEN = 4;
constexpr int ANIM_WALK_LEN = 6;
constexpr int ANIM_HURT_LEN = 3;
constexpr int ANIM_RUN_LEN = 6;


void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    int playerCount = 0;
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& fields = GamepadControllers[gamepadIdx];
        fields = {};
        fields.dinoPos = {windowSize.x / 2, windowSize.y / 2};
        fields.dinoColor = playerCount;
        playerCount++;
    }

    // Préparation du drawcall de la polyline (zigzag en fond).
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

    // Préparation du drawcall du nom en bas à droite
    {
        std::vector<DinoVertex> vs;
        textSize_Prenom = Dino_GenVertices_Text(vs, "Cedric Charrier", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Nom");
    }

    // Preparing the texture for the dino
    texID_dino = XDino_CreateGpuTexture("dinosaurs.png");
}

uint64_t DrawDino(DinoGamepad gamepad, DinoControllerFields& fields, float deltaTime)
{
    // Choosing Animation based on current player behaviour
    int currAnimLen;
    int animationSpeed;
    int firstFrameOfAnim;

    // Temp condition to test damage
    if (fields.g_dinoCanTakeDamage && gamepad.btn_left || fields.dinoDamageAnimTimer > 0.f) {
        currAnimLen = ANIM_HURT_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + 4;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;

        if (fields.g_dinoCanTakeDamage) {
            fields.dinoDamageAnimTimer = ANIM_HURT_LEN;
            fields.g_dinoCanTakeDamage = false;
        }
        else {
            fields.dinoDamageAnimTimer -= deltaTime;
            if (fields.dinoDamageAnimTimer <= 0.f) {
                fields.g_dinoCanTakeDamage = true;
            }
        }
    }
    else if (gamepad.stick_left_x == 0.0f && gamepad.stick_left_y == 0.0f) {
        currAnimLen = ANIM_IDLE_LEN;
        firstFrameOfAnim = 0;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    }
    else if (gamepad.btn_right == 0.0f) {
        currAnimLen = ANIM_WALK_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    }
    else {
        currAnimLen = ANIM_RUN_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + ANIM_HURT_LEN + 4 + 1;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND * 2;
    }

    if (fields.dinoAnimElapsed > 1.f / animationSpeed) {
        fields.currFrame = (fields.currFrame + 1) % currAnimLen;
        fields.dinoAnimElapsed = 0.0f;
    }
    else {
        fields.dinoAnimElapsed += deltaTime;
    }

    // Displaying Dino

    // Only change facing orientation if player is moving
    if (gamepad.stick_left_x != 0.0f) {
        fields.dinoGoingLeft = gamepad.stick_left_x <= 0.0f; // Changes facing orientation based on input
    }

    std::vector<DinoVertex> vs;
    vs.resize(6);

    vs[0].pos = fields.dinoGoingLeft ? UPPER_RIGHT : UPPER_LEFT;
    vs[0].u = 0 + (firstFrameOfAnim + fields.currFrame) * 24;
    vs[0].v = 0 + fields.dinoColor * 24;

    vs[1].pos = fields.dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
    vs[1].u = 24 + (firstFrameOfAnim + fields.currFrame) * 24;
    vs[1].v = 0 + fields.dinoColor * 24;

    vs[2].pos = fields.dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
    vs[2].u = 0 + (firstFrameOfAnim + fields.currFrame) * 24;
    vs[2].v = 24 + fields.dinoColor * 24;

    vs[3].pos = fields.dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
    vs[3].u = 24 + (firstFrameOfAnim + fields.currFrame) * 24;
    vs[3].v = 0 + fields.dinoColor * 24;

    vs[4].pos = fields.dinoGoingLeft ? LOWER_LEFT : LOWER_RIGHT;
    vs[4].u = 24 + (firstFrameOfAnim + fields.currFrame) * 24;
    vs[4].v = 24 + fields.dinoColor * 24;

    vs[5].pos = fields.dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
    vs[5].u = 0 + (firstFrameOfAnim + fields.currFrame) * 24;
    vs[5].v = 24 + fields.dinoColor * 24;

    fields.vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
    XDino_Draw(fields.vbufID_dino, texID_dino, fields.dinoPos, DINO_SCALE);

    return fields.vbufID_dino;
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;

    // Gestion des entrées et mise à jour de la logique de jeu.
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& fields = GamepadControllers[gamepadIdx];

        if (!fields.g_dinoCanTakeDamage)
            continue;

        // Prevent Diagonal speed boost
        if (gamepad.stick_left_x != 0.0f && gamepad.stick_left_y != 0.0f)
            fields.dinoCurrentSpeed = 0.75f * DINO_SPEED;
        else
            fields.dinoCurrentSpeed = DINO_SPEED;

        DinoVec2 dinoPosDelta = {0, 0};
        dinoPosDelta.x += gamepad.stick_left_x * (fields.dinoCurrentSpeed + gamepad.btn_right * DINO_RUN_SPEED) *
            deltaTime;
        dinoPosDelta.y += gamepad.stick_left_y * (fields.dinoCurrentSpeed + gamepad.btn_right * DINO_RUN_SPEED) *
            deltaTime;

        fields.dinoPos.x += dinoPosDelta.x;
        fields.dinoPos.y += dinoPosDelta.y;
    }

    // Affichage
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);

    // Dessin de la "polyligne"
    // XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

    // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    // DinoVec2 windowSize = XDino_GetWindowSize();
    // XDino_SetRenderSize(windowSize);

    DinoVec2 renderSize = XDino_GetRenderSize();
    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    // Affichage du nom en bas à droite
    {
        float tx = (renderSize.x - textSize_Prenom.x * 2);
        float ty = (renderSize.y - textSize_Prenom.y * 2);

        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {.x = tx, .y = ty}, 2);
    }

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {

        // if (gamepadIdx != DinoGamepadIdx::Gamepad1)
        //     continue;

        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        DinoControllerFields& fields = GamepadControllers[gamepadIdx];

        fields.vbufID_dino = DrawDino(gamepad, fields, deltaTime);
        XDino_DestroyVertexBuffer(fields.vbufID_dino);
    }

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur SHIFT.
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
    //XDino_DestroyVertexBuffer(vbufID_e);
    //XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    //XDino_DestroyGpuTexture(texID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    XDino_DestroyVertexBuffer(vbufID_prenom);
}