#include "dino_terrain.h"

#include <dino/dino_lobby.h>
#include <dino/dino_player.h>
#include <dino/dino_lasso.h>
#include <dino/dino_tree.h>
#include <dino/dino_entity.h>
#include <algorithm>

constexpr DinoColor PLAYER_COLORS[4] = {
    DinoColor_BLUE,
    DinoColor_RED,
    DinoColor_YELLOW,
    DinoColor_GREEN,
};

void Lobby_Update(double timeSinceStart, float deltaTime,
                  std::vector<PlayerState>& players,
                  std::vector<DinoGamepadIdx>& unassignedGamepads,
                  std::vector<DinoTree>& trees,
                  std::vector<DinoEntity*>& entities,
                  DinoTerrain& terrain,
                  EGameState& state)
{
    // Détection des nouveaux joueurs
    for (int i = 0; i < unassignedGamepads.size(); i++) {
        DinoGamepadIdx idx = unassignedGamepads[i];
        DinoGamepad gamepad;
        if (XDino_GetGamepad(idx, gamepad)) {
            if (gamepad.start) {
                int idxPlayer = players.size();
                if (idxPlayer < 4) {
                    players.emplace_back(idx, gamepad, idxPlayer, PLAYER_COLORS[idxPlayer]);
                    unassignedGamepads.erase(unassignedGamepads.begin() + i);
                }
                break;
            }
        }
    }

    // Update des joueurs
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

    // Update lassos
    for (PlayerState& player : players)
        player.lasso.Update(player.dino.GetPos());

    for (size_t idxA = 0; idxA < players.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < players.size(); ++idxB)
            DinoLasso::ResolveCollision(players[idxA].lasso, players[idxB].lasso);

    for (PlayerState& player : players)
        for (DinoEntity* pEntity : entities)
            if (player.lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart);

    // Transition lobby -> InGame si un arbre est bouclé
    for (DinoTree& tree : trees) {
        if (tree.WasLooped()) {
            state = EGameState::InGame;
            terrain.Shut();
            terrain.Init({480, 360}, tree.GetIdxSeason());
            trees.clear();
            break;
        }
    }
}

void Lobby_Draw(double timeSinceStart,
                std::vector<PlayerState>& players,
                std::vector<DinoEntity*>& entities)
{
    for (PlayerState& player : players)
        player.lasso.Draw();

    for (DinoEntity* pEntity : entities)
        pEntity->Draw(timeSinceStart);
}