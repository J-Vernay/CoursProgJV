/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.


#include "Animal.h"
#include "Physics.h"

#include <dino/CodeBase/dino_draw_utils.h>
#include <dino/xdino.h>
#include <format>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include "Rendering.h"
#include <array>
#include <dino/UI.h>

#include <map>

// Variables globales.
double g_lastTime = 0;
double g_rotation = 360.0;
double g_scale = 1.0;

uint64_t vbufID_polyline;
uint64_t vbufID_imageMilieu;
//uint64_t vbufID_circle;
uint64_t texID_dinoTexture;


#include <deque>
UI gUI;
DinoTerrain g_Terrain;
std::deque<Animal> animals;
float spawnClock =1;
float clockReset = 1;



constexpr DinoVec2 RENDER_SIZE = {480, 360};

std::array<DinoObject,4> PlayerList;
uint64_t texID_AnimalTexture;


static Physics physics;
static Rendering rendering;




#pragma region Les truc du prof

// myName global variable
float tx;
float ty;

const std::string myName = "VEDRENNE Warrick";

uint64_t globalVbufID;

// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

// Constantes.
constexpr float CIRCLE_SPEED = 5.f; // Nombre de pixels parcourus en une seconde.

void Dino_GameInit()
{
    XDino_SetRenderSize({480, 360});
    
    g_Terrain.Init(RENDER_SIZE, rand() % 4);


    

    
    texID_AnimalTexture = XDino_CreateGpuTexture("animals.png");
    Animal::InitStatic(texID_AnimalTexture);


    
    
    texID_dinoTexture = XDino_CreateGpuTexture("dinosaurs.png");
    XDino_SetRenderSize(RENDER_SIZE);

    for (int i = 0; i < PlayerList.size(); i++)
    {
        PlayerList[i].playerId = i;
        PlayerList[i].texID_dinoTexture = texID_dinoTexture;
        PlayerList[i].dinoPos = {XDino_GetRenderSize().x/2, XDino_GetRenderSize().y/2};
        
        PlayerList[i].Init();

        switch (i) {
            case 0:
            PlayerList[i].color = DinoColor_BLUE; break;
            case 1:
            PlayerList[i].color = DinoColor_RED; break;
            case 2:
            PlayerList[i].color = DinoColor_YELLOW; break;
            case 3:
            PlayerList[i].color = DinoColor_GREEN; break;
            
        }
    }
    
    // Affiche mon nom en bas a droite (en principe)
    {
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, myName, DinoColor_WHITE, DinoColor_GREY);
        tx = XDino_GetRenderSize().x - textSize.x * 2;
        ty = XDino_GetRenderSize().y - textSize.y * 2;

        globalVbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
    }
}

#pragma endregion

void Dino_GameFrame(double timeSinceStart)
{
#pragma region Les truc du prof
    
    
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;


    g_Terrain.Draw();
    physics.Update(deltaTime);
    gUI.Update(deltaTime);
    
   {
        
        // Affichage
        constexpr DinoColor CLEAR_COLOR = { 50, 50, 80, 255 };
        XDino_SetClearColor(CLEAR_COLOR);

        // Dessin de la "polyligne"
        XDino_Draw(vbufID_polyline, XDino_TEXID_WHITE);

        DinoVec2 renderSize = XDino_GetRenderSize();

        // Dessin de la texture centrale qu'on peut bouger.
        DinoVec2 translation = { renderSize.x / 2, renderSize.y / 2 };
        double scale = g_scale * std::min(renderSize.x, renderSize.y) / 4;

   }


    

    

    // Dessin du cercle que l'on peut bouger.
    

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
        XDino_Draw(globalVbufID, XDino_TEXID_FONT, { tx, ty }, 2);
    }




    for (int i = 0; i < animals.size(); i++)
    {
        animals[i].Update(deltaTime);
    }


    spawnClock -= deltaTime;

    if (spawnClock <= 0)
    {
        spawnClock = clockReset;

        animals.emplace_back();
        animals.back().Init(texID_AnimalTexture);
    }

    



    DinoGamepad gamepad{};
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad))
    {
        PlayerList[0].Update(timeSinceStart,deltaTime,gamepad);
    }
        
    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad)) {
        PlayerList[1].Update(timeSinceStart,deltaTime,gamepad);
    }

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad))
        PlayerList[2].Update(timeSinceStart,deltaTime,gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad))
        PlayerList[3].Update(timeSinceStart,deltaTime,gamepad);



    rendering.Update(deltaTime);
            

#pragma endregion
    //DinoObject::Update();

#if !XDINO_RELEASE

    // Affichage des statistiques si on appuie sur SHIFT.
    DinoGamepad keyboard;

    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);

    if (bKeyboardOk && keyboard.shoulder_left)
    {
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
    
    for (int i = 0; i < PlayerList.size(); i++)
    {
        PlayerList[i].DinoShut();
    }
    g_Terrain.Shut();

    Animal::ShutStatic();
    XDino_DestroyVertexBuffer(vbufID_polyline);
}