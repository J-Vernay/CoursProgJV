#include <dino/xdino.h>       

#include "dino_lobbystate.h"  
#include "dino_gamestate.h"   
#include "dino_playerstate.h"
#include "dino_tree.h"

#include <dino/dino_draw_utils.h>
#include <algorithm>
#include <format>

LobbyState::LobbyState(double timeSinceStart, const GameContext& ctx)
{
    SpawnTrees(timeSinceStart, ctx);
}

void LobbyState::SpawnTrees(double timeSinceStart, const GameContext& ctx)
{
    m_trees.clear();
    DinoVec2 terrainMin = ctx.terrain.GetTopLeft();
    DinoVec2 terrainMax = ctx.terrain.GetBottomRight();
    for (int i = 0; i < 4; ++i) {
        float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5.f);
        float y = terrainMin.y + 80.f;
        m_trees.emplace_back(DinoVec2{x, y}, i, timeSinceStart);
    }
}

void LobbyState::HandlePlayerJoining(GameContext& ctx)
{
    for (int i = 0; i < (int)ctx.unassignedGamepads.size(); ++i) {
        DinoGamepadIdx idx = ctx.unassignedGamepads[i];
        DinoGamepad gamepad;
        if (!XDino_GetGamepad(idx, gamepad))
            continue;

        if (gamepad.start) {
            int idxPlayer = (int)ctx.players.size();
            if (idxPlayer < 4) {
                ctx.players.emplace_back(idx, gamepad, PLAYER_COLORS[idxPlayer], idxPlayer, PLAYER_COLORS[idxPlayer]);
                ctx.unassignedGamepads.erase(ctx.unassignedGamepads.begin() + i);
            }
            break;
        }
    }
}

GameState* LobbyState::Update(double timeSinceStart, float deltaTime, GameContext& ctx)
{
    HandlePlayerJoining(ctx);

    for (PlayerState& player : ctx.players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad))
            player.gamepad = gamepad;
    }

    for (PlayerState& player : ctx.players)
        player.dino.Update(timeSinceStart, deltaTime, player.gamepad);

    std::vector<DinoEntity*> entities;
    for (PlayerState& player : ctx.players)
        entities.emplace_back(&player.dino);
    for (DinoTree& tree : m_trees)
        entities.emplace_back(&tree);

    DinoVec2 terrainMin = ctx.terrain.GetTopLeft();
    DinoVec2 terrainMax = ctx.terrain.GetBottomRight();

    for (size_t a = 0; a < entities.size(); ++a)
        for (size_t b = a + 1; b < entities.size(); ++b)
            DinoEntity::ResolveCollision(*entities[a], *entities[b]);

    for (DinoEntity* e : entities)
        e->ApplyLimit(terrainMin, terrainMax);

    for (PlayerState& player : ctx.players)
        player.lasso.Update(player.dino.GetPos());

    for (size_t a = 0; a < ctx.players.size(); ++a)
        for (size_t b = a + 1; b < ctx.players.size(); ++b)
            DinoLasso::ResolveCollision(ctx.players[a].lasso, ctx.players[b].lasso);

    for (PlayerState& player : ctx.players)
        for (DinoEntity* e : entities)
            if (player.lasso.WasInLoop(e->GetPos()))
                e->ReactLoop(timeSinceStart);

    for (DinoTree& tree : m_trees) {
        if (tree.WasLooped()) {
            int idxSeason = tree.GetIdxSeason();
            ctx.terrain.Shut();
            ctx.terrain.Init(ctx.renderSize, idxSeason);
            return new PlayState(timeSinceStart);
        }
    }

    return nullptr; 
}

void LobbyState::Draw(double timeSinceStart, const GameContext& ctx) const
{
    ctx.terrain.Draw(timeSinceStart);

    for (PlayerState& player : ctx.players)  
        player.lasso.Draw();

    std::vector<DinoEntity*> entities;
    for (const PlayerState& player : ctx.players)
        entities.emplace_back(const_cast<DinoPlayer*>(&player.dino));
    for (const DinoTree& tree : m_trees)
        entities.emplace_back(const_cast<DinoTree*>(&tree));

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);
    for (DinoEntity* e : entities)
        e->Draw(timeSinceStart);

    for (int i = 0; i < (int)ctx.players.size(); ++i) {
        const PlayerState& player = ctx.players[i];
        float ty = ctx.renderSize.y / 3.f + i * 30.f;
        std::string text = std::format("+{}", player.score);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, player.playerColor, DinoColor_GREY);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Player Score");
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {0, ty}, 2);
    }
}