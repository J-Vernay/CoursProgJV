/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <__msvc_ostream.hpp>
#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>

#include <format>
#include <algorithm>
#include <iostream>


// Variables globales.
double g_lastTime = 0;

std::vector<DinoPlayer> g_Players;
DinoTerrain g_Terrain;
std::vector<DinoLasso> g_Lassos;

std::vector<DinoAnimal> g_Animals;
double g_timeSpawnAnimal = 0;
float g_spawnInterval = 1;

uint64_t vbufID_prenom;
DinoVec2 textSize_prenom;


// Variable globale pour l'affichage de debug.
int g_debugScroll = 0;

constexpr DinoVec2 RENDER_SIZE = {480, 360};


void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);

    DinoPlayer::InitStatic();
    DinoAnimal::InitStatic();

    g_Players.resize(4);
    g_Players[0].Init(0);
    g_Players[1].Init(1);
    g_Players[2].Init(2);
    g_Players[3].Init(3);

    g_Lassos.resize(4);
    g_Lassos[0].Init(DinoColor_BLUE);
    g_Lassos[1].Init(DinoColor_RED);
    g_Lassos[2].Init(DinoColor_YELLOW);
    g_Lassos[3].Init(DinoColor_GREEN);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_Terrain.Init(RENDER_SIZE, idxSeason);

    // Préparation du drawcall du prénom
    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "Antoine BOULANGER", DinoColor_WHITE, DinoColor_GREY);
        vbufID_prenom = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Prenom");
    }

}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;
    g_spawnInterval -= (deltaTime/100) * 0.5;
    
    std::cout<<g_spawnInterval<<std::endl;
    XDino_SetRenderSize(RENDER_SIZE);

    // Gestion des entrées et mise à jour de la logique de jeu.

    DinoGamepad gamepad{};
    if (XDino_GetGamepad(DinoGamepadIdx::Keyboard, gamepad))
        g_Players[0].Update(timeSinceStart, deltaTime, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad1, gamepad))
        g_Players[1].Update(timeSinceStart, deltaTime, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad2, gamepad))
        g_Players[2].Update(timeSinceStart, deltaTime, gamepad);

    if (XDino_GetGamepad(DinoGamepadIdx::Gamepad3, gamepad))
        g_Players[3].Update(timeSinceStart, deltaTime, gamepad);

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();

    // Purger les animaux qui sont morts.
    // /!\ std::remove ne supprime pas /!\ il déplace à la fin du tableau
    // Il faut ensuite appeler .erase() pour enlever les éléments.
    auto it = std::remove_if(g_Animals.begin(), g_Animals.end(), DinoAnimal::IsDead);
    g_Animals.erase(it, g_Animals.end());

    // Spawner un animal si besoin.
    if ((timeSinceStart > g_timeSpawnAnimal)) {
        DinoAnimal& animal = g_Animals.emplace_back();
        auto kind = static_cast<EAnimalKind>(XDino_RandomInt32(0, 7));

        float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
        float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);

        animal.Init(timeSinceStart, kind, {x, y});
        g_timeSpawnAnimal = timeSinceStart + g_spawnInterval;
    }

    // Update les animaux.
    for (DinoAnimal& animal : g_Animals)
        animal.Update(timeSinceStart, deltaTime);

    // Pointeur de DinoEntity peut pointer vers DinoPlayer/DinoAnimal
    // car il y a un lien d'héritage.
    std::vector<DinoEntity*> entities;
    for (DinoPlayer& player : g_Players)
        entities.emplace_back(&player);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);

    for (size_t idxA = 0; idxA < entities.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
            DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

    for (DinoEntity* pEntity : entities)
        pEntity->ApplyLimit(terrainMin, terrainMax);

    if (g_Lassos.size() != g_Players.size())
        DINO_CRITICAL("Il devrait y avoir autant de lassos que de joueurs");
    for (int i = 0; i < g_Lassos.size(); ++i)
        g_Lassos[i].Update(g_Players[i].GetPos());

    for (size_t idxA = 0; idxA < g_Lassos.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < g_Lassos.size(); ++idxB)
            DinoLasso::ResolveCollision(g_Lassos[idxA], g_Lassos[idxB]);

    for (DinoLasso& lasso : g_Lassos)
        for (DinoEntity* pEntity : entities)
            if (lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart);

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    // Affichage

    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};

    XDino_SetClearColor(CLEAR_COLOR);

    g_Terrain.Draw(timeSinceStart);

    for (DinoLasso& lasso : g_Lassos)
        lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

    // Affiche le prénom.
    {
        float tx = (RENDER_SIZE.x - textSize_prenom.x * 2);
        float ty = (RENDER_SIZE.y - textSize_prenom.y * 2);
        XDino_Draw(vbufID_prenom, XDino_TEXID_FONT, {tx, ty}, 2);
    }

    {
        double time = (std::ceil(timeSinceStart * 100.0) / 100.0);
        std::string text = std::to_string(time);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        uint64_t vbufID = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Time");
        XDino_Draw(vbufID, XDino_TEXID_FONT, {RENDER_SIZE.x - 50, 0}, 2);
        XDino_DestroyVertexBuffer(vbufID);
    }

#if !XDINO_RELEASE
    // Affichage des statistiques si on appuie sur CTRL.
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
    // For-range loop
    for (DinoPlayer& player : g_Players)
        player.Shut();
    for (DinoAnimal& animal : g_Animals)
        animal.Shut();
    g_Terrain.Shut();

    DinoPlayer::ShutStatic();
    DinoAnimal::ShutStatic();

    XDino_DestroyVertexBuffer(vbufID_prenom);
}