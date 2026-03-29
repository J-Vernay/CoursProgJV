#include <dino/dino_ingame.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <dino/dino_entity.h>
#include <dino/dino_terrain.h>
#include "dino_game.h"
#include "dino_player.h"

#include <algorithm>

constexpr double SPAWNTIME_BEGIN = 1;
constexpr double SPAWNTIME_END = 0.033;
constexpr double CHRONO_INIT = 60;

void InGame_Update(double timeSinceStart, float deltaTime,
                   std::vector<PlayerState>& players,
                   std::vector<DinoAnimal>& animals,
                   std::vector<DinoEntity*>& entities,
                   DinoTerrain& terrain,
                   double& chrono,
                   double& timeSpawnAnimal,
                   bool bPressedStart,
                   bool& bWasStartPressed,
                   EGameState& state)
{
    // Transition vers pause
    if (bPressedStart && !bWasStartPressed)
        state = EGameState::Paused;
    bWasStartPressed = bPressedStart;

    // Purge des animaux morts
    auto it = std::remove_if(animals.begin(), animals.end(), DinoAnimal::IsDead);
    for (auto it2 = it; it2 < animals.end(); ++it2)
        it2->Shut();
    animals.erase(it, animals.end());

    // Spawn animal
    // Spawn animal
    if (timeSinceStart > timeSpawnAnimal) {
        auto kind = static_cast<EAnimalKind>(XDino_RandomInt32(0, 7));
        DinoVec2 terrainMin = terrain.GetTopLeft();
        DinoVec2 terrainMax = terrain.GetBottomRight();
        float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
        float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);
        animals.emplace_back(timeSinceStart, kind, DinoVec2{x, y});
        double spawnTime = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * chrono;
        timeSpawnAnimal = timeSinceStart + spawnTime;

        // Reconstruire entities car emplace_back a pu réallouer g_Animals
        entities.clear();
        for (PlayerState& player : players)
            entities.emplace_back(&player.dino);
        for (DinoAnimal& animal : animals)
            entities.emplace_back(&animal);
    }

    // Update animaux
    for (DinoAnimal& animal : animals)
        animal.Update(timeSinceStart, deltaTime);

    // Update joueurs
    for (PlayerState& player : players)
        player.dino.Update(timeSinceStart, deltaTime, player.gamepad);

    // Collisions
    for (size_t idxA = 0; idxA < entities.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < entities.size(); ++idxB)
            DinoEntity::ResolveCollision(*entities[idxA], *entities[idxB]);

    DinoVec2 terrainMin = terrain.GetTopLeft();
    DinoVec2 terrainMax = terrain.GetBottomRight();
    for (DinoEntity* pEntity : entities)
        pEntity->ApplyLimit(terrainMin, terrainMax);

    // Lassos
    for (PlayerState& player : players)
        player.lasso.Update(player.dino.GetPos());

    for (size_t idxA = 0; idxA < players.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < players.size(); ++idxB)
            DinoLasso::ResolveCollision(players[idxA].lasso, players[idxB].lasso);

    for (PlayerState& player : players)
        for (DinoEntity* pEntity : entities)
            if (player.lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart);

    // Chrono
    chrono -= deltaTime;
}

void InGame_Draw(double timeSinceStart,
                 std::vector<PlayerState>& players,
                 std::vector<DinoEntity*>& entities)
{
    for (PlayerState& player : players)
        player.lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);
}