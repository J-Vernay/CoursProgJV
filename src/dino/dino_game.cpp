/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/dino_animal.h>
#include <dino/dino_geometry.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>

#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include <vector>
#include <format>

// Variables globales.
double lastTime = 0;

std::vector<DinoPlayer> g_Dinos;
DinoTerrain g_Terrain;

std::vector<DinoAnimal> g_Animals;
double g_AnimalSpawnTime = 0;

void Dino_GameInit()
{
    DinoVec2 rdrSize = {480, 360};
    XDino_SetRenderSize(rdrSize);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.25f, rdrSize.y * 0.25f}, DinoGamepadIdx::Keyboard, 0, DinoColor_BLUE);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.75f, rdrSize.y * 0.25f}, DinoGamepadIdx::Gamepad1, 1, DinoColor_RED);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.25f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad2, 2, DinoColor_YELLOW);
    g_Dinos.emplace_back().Init({rdrSize.x * 0.75f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad3, 3, DinoColor_GREEN);

    g_Terrain.Init(24, 16);
}

void Dino_GameFrame(double timeSinceStart)
{
    XDino_ProfileBegin(DinoColor_RED, "Dino_GameFrame");

    DinoVec2 rdrSize = XDino_GetRenderSize();

    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    // Mise à jour

    for (DinoPlayer& dino : g_Dinos)
        dino.Update(timeSinceStart, deltaTime);

    if (timeSinceStart - g_AnimalSpawnTime >= 1) {
        g_AnimalSpawnTime = timeSinceStart;
        DinoVec2 spawnPos = g_Terrain.GenerateRandomSpawn();
        g_Animals.emplace_back().InitRandom(spawnPos, timeSinceStart);
    }
    for (DinoAnimal& animal : g_Animals)
        animal.Update(timeSinceStart, deltaTime);

    // Lassos

    for (DinoPlayer& dino : g_Dinos) {
        std::span<DinoVec2 const> lassoPoints = dino.GetLasso();
        size_t lassoSize = lassoPoints.size();
        if (lassoSize < 4)
            continue;
        // On n'a besoin de regarder que le dernier segment qui a été rajouté.
        DinoVec2 C = lassoPoints[lassoSize - 2];
        DinoVec2 D = lassoPoints[lassoSize - 1];
        for (size_t i = 0; i < lassoSize - 3; ++i) {
            DinoVec2 A = lassoPoints[i];
            DinoVec2 B = lassoPoints[i + 1];
            if (Dino_IntersectSegment(A, B, C, D)) {
                dino.EraseLasso(i + 1, lassoSize - 1);
                // À partir d'ici, "lassoPoints" est invalide!
                break;
            }
        }

    }

    // Affichage

    g_Terrain.Draw(timeSinceStart, deltaTime);

    DinoDrawCall drawAnimals;
    drawAnimals.textureName = "animals.png";
    for (DinoAnimal const& animal : g_Animals)
        animal.AddDrawCall(timeSinceStart, deltaTime, drawAnimals);
    XDino_Draw(drawAnimals);

    for (DinoPlayer const& player : g_Dinos)
        XDino_Draw(player.DrawCallLasso());

    DinoDrawCall drawDinos = DinoPlayer::DrawCallDinos(g_Dinos, timeSinceStart, deltaTime);
    XDino_Draw(drawDinos);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        XDino_Draw(drawCall);
    }

    XDino_ProfileEnd();
}

void Dino_GameShut()
{

}