/// @file dino_game.cpp
/// @brief Implémentation des fonctions principales de la logique de jeu.

#include <format>
#include <algorithm>
#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>

// Variables globales.
double lastTime = 0;
DinoTerrain terrain;
std::vector<DinoPlayer> players;
std::vector<DinoLasso> lassos;
std::vector<DinoAnimal> animals;
std::vector<DinoActor*> actors;
std::vector<DinoGamepadIdx> availableGamepads;
std::vector<int> scores;
float spawnTimer = 60;
float gameTimer = 60;

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
    XDino_ProfileBegin(DinoColor_GREEN, "Game logic");
    // Prendre en compte le temps qui passe.

    float deltaTime = static_cast<float>(timeSinceStart - lastTime);
    lastTime = timeSinceStart;

    gameTimer -= deltaTime;
    gameTimer = std::max(gameTimer, 0.0f);

    if (players.size() < 4) {
        for (DinoGamepadIdx gamepadIdx : availableGamepads) {
            DinoGamepad gamepad;
            XDino_GetGamepad(gamepadIdx, gamepad);

            if (abs(gamepad.stick_left_x) + abs(gamepad.stick_left_y) < 0.1f)
                continue;

            DinoVec2 windowSize = XDino_GetRenderSize();
            players.push_back(DinoPlayer({windowSize.x / 2, windowSize.y / 2},
                                         gamepadIdx,
                                         static_cast<DinoPlayer::Color>(players.size())));
            lassos.push_back(DinoLasso());
            scores.push_back(0);
            availableGamepads.erase(std::find(availableGamepads.begin(), availableGamepads.end(), gamepadIdx));
        }
    }
    
    // Gestion des entrées et mise à jour de la logique de jeu.
    terrain.update(deltaTime);

    {
        spawnTimer += deltaTime;
        if (spawnTimer >= std::max(0.5f, gameTimer / 30.0f)) {
            spawnTimer = 0;

            DinoVec2 minPosition = terrain.get_terrain_min_position();
            DinoVec2 maxPosition = terrain.get_terrain_max_position();

            auto spawnedAnimal = DinoAnimal({XDino_RandomFloat(minPosition.x, maxPosition.x),
                                             XDino_RandomFloat(minPosition.y, maxPosition.y)});

            bool spawned = false;

            for (int i = 0; i < animals.size(); i++) {
                if (animals[i].isAlive())
                    continue;
                animals[i] = spawnedAnimal;
                spawned = true;
                break;
            }

            if (!spawned) {
                animals.push_back(spawnedAnimal);
            }
        }
    }

    {
        actors.clear();
        for (DinoPlayer& player : players) {
            actors.push_back(&player);
        }

        for (DinoAnimal& animal : animals) {
            actors.push_back(&animal);
        }

        for (DinoActor* actor : actors) {
            actor->update(deltaTime);
        }
        
        XDino_ProfileBegin(DinoColor_YELLOW, "Collision solving");
        if (actors.size() > 1) {
            auto actorIterator = actors.begin();
            while (actorIterator < actors.end() - 1) {
                auto otherIterator = actorIterator + 1;
                while (otherIterator < actors.end()) {
                    actorIterator[0]->handleActorCollision(otherIterator[0]);
                    ++otherIterator;
                }
                ++actorIterator;
            }
        }

        for (DinoActor* actor : actors) {
            actor->handleTerrainCollision();
        }
        XDino_ProfileEnd();

        XDino_ProfileBegin(DinoColor_RED, "Lasso intersections");
        for (int i = 0; i < lassos.size(); i++) {
            for (int j = 0; j < lassos.size(); j++) {
                if (i == j)
                    continue;
                lassos[i].handleActorCollision(&players[j]);
            }

            lassos[i].update(deltaTime, &players[i]);
            lassos[i].handleSelfIntersection([i](auto first, auto second) {
                int bonus = 1;
                for (DinoActor* actor : actors) {
                    if (actor == &players[i])
                        continue;

                    if (!actor->handleActorCircled(first, second)) continue;
                    scores[i] += (bonus++) * 10;
                }
            });
        }
        XDino_ProfileEnd();
    }

    XDino_ProfileEnd();
    // Affichage
    // On veut avoir une correspondance 1:1 entre pixels logiques et pixels à l'écran.
    XDino_SetRenderSize({480, 360});

    terrain.draw_ocean();
    XDino_ProfileBegin(DinoColor_GREEN, "Draw");
    terrain.draw_terrain();

    {
        for (int i = 0; i < lassos.size(); i++) {
            lassos[i].draw(players[i].getColor());
        }
    }

    {
        std::sort(actors.begin(), actors.end(), DinoPlayer::compareHeight);
        for (DinoActor* actor : actors) {
            actor->draw();
        }
    }
    XDino_ProfileEnd();

    // Nombre de millisecondes qu'il a fallu pour afficher la frame précédente.
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_GREY, DinoColor_WHITE);
        XDino_Draw(drawCall);
    }

    {
        int seconds = static_cast<int>(floorf(gameTimer));
        float cents = floorf(fmodf(gameTimer, 1.0f) * 100);
        std::string text = std::format("{:}:{:02}", seconds, cents);

        DinoVec2 size = {};
        DinoVec2 renderSize = XDino_GetRenderSize();
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_GREY, DinoColor_WHITE, &size);
        drawCall.scale = 2;
        drawCall.translation = {(renderSize.x - size.x * static_cast<float>(drawCall.scale)) / 2, 0};
        XDino_Draw(drawCall);
    }

    {
        DinoVec2 renderSize = XDino_GetRenderSize();
        DinoVec2 size = {};

        Dino_CreateDrawCall_Text("000", DinoColor_WHITE, DinoColor_GREY, &size);

        size.y *= 2;
        
        constexpr float scoreSpacing = 10;
        float totalScoresSize = size.y * static_cast<float>(scores.size()) + scoreSpacing * static_cast<float>(scores.size() - 1);
        float offset = (renderSize.y - totalScoresSize) / 2;
        float step = size.y + scoreSpacing;
        
        for (int i = 0; i < scores.size(); i++) {
            std::string text = std::format("{:03}", scores[i]);
            DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, players[i].getColor(), DinoColor_WHITE, &size);
            drawCall.translation = {0, offset + step * static_cast<float>(i)};
            drawCall.scale = 2;
            XDino_Draw(drawCall);
        }
    }

    {
        std::string text = "GUEROULT Matys";
        DinoVec2 size = {};
        DinoVec2 renderSize = XDino_GetRenderSize();
        DinoDrawCall drawCall = Dino_CreateDrawCall_Text(text, DinoColor_GREY, DinoColor_WHITE, &size);
        drawCall.translation = {renderSize.x - size.x,
                                renderSize.y - size.y};
        XDino_Draw(drawCall);
    }
}

void Dino_GameShut()
{
}