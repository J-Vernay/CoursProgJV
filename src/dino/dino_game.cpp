/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <dino/xdino.h>
#include <dino/dino_animal.h>
#include <dino/dino_geometry.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_lasso.h>

#include <vector>
#include <format>

// Variables globales.
double lastTime = 0;

size_t g_PlayerCount = 0;
std::vector<DinoPlayer> g_Dinos;
std::vector<DinoLasso> g_Lassos;
DinoTerrain g_Terrain;

std::vector<DinoAnimal> g_Animals;
double g_AnimalSpawnTime = 0;

void Dino_GameInit()
{
    DinoVec2 rdrSize = {480, 360};
    XDino_SetRenderSize(rdrSize);

    g_PlayerCount = 4;
    g_Dinos.resize(g_PlayerCount);
    g_Lassos.resize(g_PlayerCount);

    g_Dinos[0].Init({rdrSize.x * 0.25f, rdrSize.y * 0.25f}, DinoGamepadIdx::Keyboard, 0);
    g_Lassos[0].Init(DinoColor_BLUE);
    g_Dinos[1].Init({rdrSize.x * 0.75f, rdrSize.y * 0.25f}, DinoGamepadIdx::Gamepad1, 1);
    g_Lassos[1].Init(DinoColor_RED);
    g_Dinos[2].Init({rdrSize.x * 0.25f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad2, 2);
    g_Lassos[2].Init(DinoColor_YELLOW);
    g_Dinos[3].Init({rdrSize.x * 0.75f, rdrSize.y * 0.75f}, DinoGamepadIdx::Gamepad3, 3);
    g_Lassos[3].Init(DinoColor_GREEN);

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

    XDino_ProfileBegin(DinoColor_BLUE, "Players logic");
    for (size_t idxPlayer = 0; idxPlayer < g_PlayerCount; ++idxPlayer)
        g_Dinos[idxPlayer].Update(timeSinceStart, deltaTime);
    XDino_ProfileEnd();

    XDino_ProfileBegin(DinoColor_GREEN, "Animals logic");
    if (timeSinceStart - g_AnimalSpawnTime >= 1) {
        g_AnimalSpawnTime = timeSinceStart;
        DinoVec2 spawnPos = g_Terrain.GenerateRandomSpawn();
        g_Animals.emplace_back().InitRandom(spawnPos, timeSinceStart);
    }

    for (DinoAnimal& animal : g_Animals) {
        animal.Update(timeSinceStart, deltaTime);
    }
    XDino_ProfileEnd();

    XDino_ProfileBegin(DinoColor_YELLOW, std::format("Physics animals= {}", g_Animals.size()).c_str());

    for (DinoPlayer& dino1 : g_Dinos)
        for (DinoPlayer& dino2 : g_Dinos)
            DinoEntity::ResolveCollision(dino1, dino2);

    for (DinoAnimal& animal1 : g_Animals)
        for (DinoPlayer& dino2 : g_Dinos)
            DinoEntity::ResolveCollision(animal1, dino2);

    for (DinoAnimal& animal1 : g_Animals)
        for (DinoAnimal& animal2 : g_Animals)
            DinoEntity::ResolveCollision(animal1, animal2);

    for (DinoPlayer& dino : g_Dinos)
        dino.ClampToTerrain(g_Terrain);

    for (DinoAnimal& animal : g_Animals) {
        DinoVec2 posBefore = animal.GetPos();
        animal.ClampToTerrain(g_Terrain);
        DinoVec2 posAfter = animal.GetPos();
        if (posAfter.x != posBefore.x || posAfter.y != posBefore.y)
            animal.SetRandomDir();
    }
    XDino_ProfileEnd();

    XDino_ProfileBegin(DinoColor_YELLOW, "Lassos logic");
    for (size_t idxPlayer = 0; idxPlayer < g_PlayerCount; ++idxPlayer) {
        g_Lassos[idxPlayer].Update(g_Dinos[idxPlayer].GetPos());
        g_Lassos[idxPlayer].ApplyCollisionSelf();
    }

    for (DinoLasso const& lasso1 : g_Lassos)
        for (DinoLasso& lasso2 : g_Lassos)
            if (&lasso1 != &lasso2)
                lasso1.ApplyCollisionTo(lasso2);
    XDino_ProfileEnd();

    // Affichage

    XDino_ProfileBegin(DinoColor_GREY, "Draw call emission");
    g_Terrain.Draw(timeSinceStart, deltaTime);

    for (DinoLasso const& lasso : g_Lassos)
        lasso.Draw();

    DinoDrawCall drawAnimals;
    drawAnimals.textureName = "animals.png";
    for (DinoAnimal const& animal : g_Animals)
        animal.AddDrawCall(timeSinceStart, deltaTime, drawAnimals);
    XDino_Draw(drawAnimals);

    DinoDrawCall drawDinos = DinoPlayer::DrawCallDinos(g_Dinos, timeSinceStart, deltaTime);
    XDino_Draw(drawDinos);

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        XDino_Draw(drawCall);
    }
    XDino_ProfileEnd();

    XDino_ProfileEnd();
}

void Dino_GameShut()
{

}