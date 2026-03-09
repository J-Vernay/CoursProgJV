/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>

#include <format>
#include <map>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;
DinoVec2 g_circlePos = {};

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
//uint64_t vbufID_circle;
uint64_t texID_imageMilieu;

uint64_t texID_dinoTexture;
uint64_t vbufID_dinoTexture;


//-------------------------------AnimVariable

#include <array>

bool isGoingRigth;


enum DinoState
{
    idle,      // first 4 frame
    walk,      // second 6 frame
    running,   // third 3 frame
    damage     // fourth 7
};

struct AnimInfo
{
    int animPosStart;
    int animFramNumber;
    int animRefresh = 8;
};

const std::map<DinoState, AnimInfo> animeStateInfo =
{
    { idle,    { 0, 4 } },
    { walk,    { 96, 6 } },
    { running, { 336, 3 , 16} },
    { damage,  { 432, 7 } }
};

#pragma region Les truc du prof
//myName global variable
float tx;
float ty;
const std::string myName = "VEDRENNE Warrick";
uint64_t globalVbufID;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);
    g_circlePos = {windowSize.x / 2, windowSize.y / 2};

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

    // Préparation du drawcall de l'image au milieu qu'on peut tourner.
    {
        constexpr DinoColor PURPLE{0x7F, 0x58, 0xAF, 0xFF};
        constexpr DinoColor CYAN{0x64, 0xC5, 0xEB, 0xFF};
        constexpr DinoColor PINK{0xE8, 0x4D, 0x8A, 0xFF};
        constexpr DinoColor ORANGE{0xFE, 0xB3, 0x26, 0xFF};

        texID_imageMilieu = XDino_CreateGpuTexture("animals.png");
        DinoVec2 texSize = XDino_GetGpuTextureSize(texID_imageMilieu);

        std::vector<DinoVertex> vs;
        vs.resize(6);
        vs[0].pos = {-2, -1};
        vs[0].color = PURPLE;
        vs[1].pos = {2, -1};
        vs[1].color = CYAN;
        vs[2].pos = {-2, 1};
        vs[2].color = PINK;
        
        vs[3].pos = {2, -1};
        vs[3].color = CYAN;
        vs[4].pos = {-2, 1};
        vs[4].color = PINK;
        vs[5].pos = {2, 1};
        vs[5].color = ORANGE;
        
        vs[0].u = 0;
        vs[0].v = 0;
        vs[1].u = texSize.x;
        vs[1].v = 0;
        vs[2].u = 0;
        vs[2].v = texSize.y;
        vs[3].u = texSize.x;
        vs[3].v = 0;
        vs[4].u = 0;
        vs[4].v = texSize.y;
        vs[5].u = texSize.x;
        vs[5].v = texSize.y;

        vbufID_imageMilieu = XDino_CreateVertexBuffer(vs.data(), vs.size(), "ImageMilieu");
    }
    texID_dinoTexture = XDino_CreateGpuTexture("dinosaurs.png");

    
    
    //Affiche mon nom en bas a droite (en principe)
    {
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, myName, DinoColor_WHITE, DinoColor_GREY);
        tx = XDino_GetRenderSize().x-textSize.x*2;
        ty = XDino_GetRenderSize().y-textSize.y*2;
        globalVbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
    }
}

#pragma endregion 
void Dino_GameFrame(double timeSinceStart)
{
#pragma region Les truc du prof
        // Prendre en compte le temps qui passe.

        float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
        g_lastTime = timeSinceStart;

        // Gestion des entrées et mise à jour de la logique de jeu.

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
        
            float DINO_SPEED= CIRCLE_SPEED;

        
            bool isRunning = gamepad.btn_right;
        
            g_circlePos.x += isRunning? gamepad.stick_left_x * DINO_SPEED * deltaTime * 3: gamepad.stick_left_x * DINO_SPEED * deltaTime;
            g_circlePos.y += isRunning ? gamepad.stick_left_y * DINO_SPEED * deltaTime * 3: gamepad.stick_left_y * DINO_SPEED * deltaTime;
        

            g_circlePos.x += gamepad.stick_left_x * CIRCLE_SPEED * deltaTime;
            g_circlePos.y += gamepad.stick_left_y * CIRCLE_SPEED * deltaTime;
        

        // Affichage

        constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

        XDino_SetClearColor(CLEAR_COLOR);

        // Dessin de la "polyligne"
        XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

        // Si on veut une correspondance 1:1 entre pixels logiques et pixels à l'écran.
        // DinoVec2 windowSize = XDino_GetWindowSize();
        // XDino_SetRenderSize(windowSize);
        DinoVec2 renderSize = XDino_GetRenderSize();

        // Dessin de la texture centrale qu'on peut bouger.
        DinoVec2 translation = {renderSize.x / 2, renderSize.y / 2};
        double scale = g_scale * std::min(renderSize.x, renderSize.y) / 4;
        XDino_Draw(vbufID_imageMilieu, texID_imageMilieu, translation, scale, g_rotation);
    } // Les truc du prof
    //-------------------------------------------------------------MyThings
    
   


    
    // Dessin du cercle que l'on peut bouger.
    XDino_Draw(vbufID_dinoTexture, texID_dinoTexture, g_circlePos,24);




    

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    {
       
        
        XDino_Draw(globalVbufID, XDino_TEXID_FONT, {tx,ty}, 2);
        //XDino_DestroyVertexBuffer(vbufID);
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

#pragma endregion 
    
    {

            {
                bool isInputLeft;
                bool isInputRight;
                for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL)
                {
                    DinoGamepad gamepad{};
                    bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
                    if (!bSuccess)
                        continue;
            
                    isInputLeft = gamepad.dpad_left;
                    isInputRight=gamepad.dpad_right;
        
                    if ( isInputRight && !isGoingRigth)isGoingRigth = true;
                    if ( isInputLeft && isGoingRigth)isGoingRigth = false;
                }
            } // LeftORigthHandler

        

        

            std::vector<DinoVertex> dinoVS;
            dinoVS.resize(6);
            dinoVS[0].pos = {-1, -1};
            dinoVS[0].color = DinoColor_WHITE;
            dinoVS[1].pos = {1, -1};
            dinoVS[1].color = DinoColor_WHITE;
            dinoVS[2].pos = {-1, 1};
            dinoVS[2].color = DinoColor_WHITE;
        
            dinoVS[3].pos = {1, -1};
            dinoVS[3].color = DinoColor_WHITE;
            dinoVS[4].pos = {-1, 1};
            dinoVS[4].color = DinoColor_WHITE;
            dinoVS[5].pos = {1, 1};
            dinoVS[5].color = DinoColor_WHITE;
        
            //-----------------------Top Triangle
            dinoVS[0].u = isGoingRigth ? 0  : 24;
            dinoVS[0].v = 0;

            dinoVS[1].u = isGoingRigth ? 24 : 0;
            dinoVS[1].v = 0;

            dinoVS[2].u = isGoingRigth ? 0  : 24;
            dinoVS[2].v = 24;

            //-----------------------
            dinoVS[3].u = isGoingRigth ? 24 : 0;
            dinoVS[3].v = 0;

            dinoVS[4].u = isGoingRigth ? 0  : 24;
            dinoVS[4].v = 24;

            dinoVS[5].u = isGoingRigth ? 24 : 0;
            dinoVS[5].v = 24;

            //-----------------------Bot Triangle

            vbufID_dinoTexture = XDino_CreateVertexBuffer(dinoVS.data(), dinoVS.size(), "dinoTexture");
    }
}

void Dino_GameShut()
{
    XDino_DestroyVertexBuffer(vbufID_imageMilieu);
    XDino_DestroyVertexBuffer(vbufID_polyline);
    
    XDino_DestroyGpuTexture(texID_imageMilieu);

    XDino_DestroyGpuTexture(texID_dinoTexture);
    XDino_DestroyGpuTexture(vbufID_dinoTexture);
}