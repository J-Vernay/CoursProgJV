/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <algorithm>
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>

#include <format>

// Variables globales.
double lastTime = 0;
DinoTerrain terrain;
std::vector<DinoActor*> actors;
std::vector<DinoGamepadIdx> availableGamepads;
int playerCount = 0;
float spawnTimer = 0;

void Dino_GameInit()
{
    DinoVec2 windowSize = XDino_GetWindowSize();
    XDino_SetRenderSize(windowSize);

    availableGamepads.push_back(DinoGamepadIdx::Gamepad1);
    availableGamepads.push_back(DinoGamepadIdx::Gamepad2);
    availableGamepads.push_back(DinoGamepadIdx::Gamepad3);
    availableGamepads.push_back(DinoGamepadIdx::Gamepad4);
    availableGamepads.push_back(DinoGamepadIdx::Keyboard);

    terrain.init();
}

void Dino_GameFrame(double timeSinceStart)
{
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    if (playerCount < 4) {
        for (DinoGamepadIdx gamepadIdx : availableGamepads) {
            DinoGamepad gamepad;
            XDino_GetGamepad(gamepadIdx, gamepad);

            if (abs(gamepad.stick_left_x) + abs(gamepad.stick_left_y) < 0.1f)
                continue;
            
            DinoVec2 windowSize = XDino_GetRenderSize();
            actors.push_back(new DinoPlayer({windowSize.x / 2, windowSize.y / 2},
                                         gamepadIdx, static_cast<int16_t>(playerCount++)));
            availableGamepads.erase(std::find(availableGamepads.begin(), availableGamepads.end(), gamepadIdx));
        }
    }

    // Gestion des entrées et mise à jour de la logique de jeu.
    terrain.update(deltaTime);

    {
        spawnTimer += deltaTime;
        if (spawnTimer >= 1) {
            spawnTimer = 0;

            DinoVec2 minPosition = terrain.get_terrain_min_position();
            DinoVec2 maxPosition = terrain.get_terrain_max_position();

            actors.push_back(new DinoAnimal({XDino_RandomFloat(minPosition.x, maxPosition.x),
                                          XDino_RandomFloat(minPosition.y, maxPosition.y)}));
        }
    }

    for (DinoActor* actor : actors) {
        actor->update(deltaTime);
    }

    // Affichage
    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    XDino_SetRenderSize({480, 360});

    terrain.draw_ocean();
    terrain.draw_terrain();

    {
        std::sort(actors.begin(), actors.end(), DinoPlayer::compareHeight);
        for (DinoActor* actor : actors) {
            actor->draw();
        }
    }

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY);
        XDino_Draw(drawCall);
    }

    {
        std::string text = "GUEROULT Matys";
        DinoVec2 size = {};
        DinoVec2 renderSize = XDino_GetRenderSize();
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_WHITE, DinoColor_GREY, &size);
        drawCall.translation = {renderSize.x - size.x,
                                renderSize.y - size.y};
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{

}