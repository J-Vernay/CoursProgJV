#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>

#include "dino_playerstate.h"
#include "dino_lobbystate.h"

#include <algorithm>
#include <format>

PlayState::PlayState(double timeSinceStart)
    : m_chrono(CHRONO_INIT)
    , m_timeSpawnAnimal(timeSinceStart)
{}

GameState* PlayState::Update(double timeSinceStart, float deltaTime, GameContext& ctx)
{
    bool bPressedStart = false;
    for (PlayerState& player : ctx.players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad)) {
            player.gamepad  = gamepad;
            bPressedStart  |= gamepad.start;
        }
    }

    if (bPressedStart && !m_bWasStartPressed)
        m_bPause = !m_bPause;
    m_bWasStartPressed = bPressedStart;

    if (m_bPause) {
        UpdatePauseMenu(deltaTime, ctx);
        return nullptr;
    }

    UpdateGameplay(timeSinceStart, deltaTime, ctx);

    if (m_chrono <= 0.0) {
        for (DinoAnimal& animal : m_animals)
            animal.Shut();
        return new LobbyState(timeSinceStart, ctx);
    }

    return nullptr;
}

void PlayState::UpdatePauseMenu(float /*deltaTime*/, GameContext& ctx)
{
    bool bUp = false, bDown = false, bLeft = false, bRight = false, bConfirm = false;
    for (PlayerState& player : ctx.players) {
        DinoGamepad gp;
        if (!XDino_GetGamepad(player.gamepadIdx, gp)) continue;
        bUp      |= gp.dpad_up;
        bDown    |= gp.dpad_down;
        bLeft    |= gp.dpad_left;
        bRight   |= gp.dpad_right;
        bConfirm |= gp.btn_right;
    }

    if (bUp && !m_bWasDpadPressed) {
        m_pauseMenuIdx = (m_pauseMenuIdx - 1 + 4) % 4;
    } else if (bDown && !m_bWasDpadPressed) {
        m_pauseMenuIdx = (m_pauseMenuIdx + 1) % 4;
    }
    m_bWasDpadPressed = bUp || bDown;

    if (m_pauseMenuIdx == 2) {
        if (bLeft)  m_chrono -= 10.0;
        if (bRight) m_chrono += 10.0;
    }

    if (!bConfirm) return;

    switch (m_pauseMenuIdx) {
        case 0: // RESET
            m_chrono = CHRONO_INIT;
            m_animals.clear();
            for (PlayerState& player : ctx.players)
                player.dino.Reset();
            break;
        case 1: // LOBBY
            for (PlayerState& player : ctx.players)
                player.score = 0;
            m_bPause = false;
            m_chrono = 0.0; 
            break;
        case 2: // CHRONO 
            break;
        case 3: // REPRENDRE
            m_bPause = false;
            break;
        default:
            break;
    }
}

void PlayState::UpdateGameplay(double timeSinceStart, float deltaTime, GameContext& ctx)
{
    DinoVec2 terrainMin = ctx.terrain.GetTopLeft();
    DinoVec2 terrainMax = ctx.terrain.GetBottomRight();

    for (PlayerState& player : ctx.players)
        player.dino.Update(timeSinceStart, deltaTime, player.gamepad);

    auto deadIt = std::remove_if(m_animals.begin(), m_animals.end(), DinoAnimal::IsDead);
    for (auto it = deadIt; it != m_animals.end(); ++it)
        it->Shut();
    m_animals.erase(deadIt, m_animals.end());
    
    auto notifIt = std::remove_if(m_scoreNotifs.begin(), m_scoreNotifs.end(),
        [timeSinceStart](const ScoreNotif& n) {
            return timeSinceStart - n.spawnTime > 2.0;
        });
    m_scoreNotifs.erase(notifIt, m_scoreNotifs.end());

    if (timeSinceStart > m_timeSpawnAnimal) {
        EAnimalKind kind = (EAnimalKind)XDino_RandomInt32(0, 7);
        float x = XDino_RandomFloat(terrainMin.x, terrainMax.x);
        float y = XDino_RandomFloat(terrainMin.y, terrainMax.y);
        m_animals.emplace_back(timeSinceStart, kind, DinoVec2{x, y});

        double interval = SPAWNTIME_END + ((SPAWNTIME_BEGIN - SPAWNTIME_END) / CHRONO_INIT) * m_chrono;
        m_timeSpawnAnimal = timeSinceStart + interval;
    }

    for (DinoAnimal& animal : m_animals)
        animal.Update(timeSinceStart, deltaTime);

    std::vector<DinoEntity*> entities;
    for (PlayerState& player : ctx.players)
        entities.emplace_back(&player.dino);
    for (DinoAnimal& animal : m_animals)
        entities.emplace_back(&animal);

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

    for (PlayerState& player : ctx.players) {
        int countPerSpecies[4] = {0, 0, 0, 0};
        for (DinoAnimal& animal : m_animals) {
            if (!player.lasso.WasInLoop(animal.GetPos())) continue;

            int espece = (int)animal.GetKind() / 2;
            countPerSpecies[espece]++;
            int points = countPerSpecies[espece] * 10;
            player.score += points;

            m_scoreNotifs.push_back({points, animal.GetPos(), player.playerColor, timeSinceStart});
        }
    }

    for (PlayerState& player : ctx.players)
        for (DinoEntity* e : entities)
            if (player.lasso.WasInLoop(e->GetPos()))
                e->ReactLoop(timeSinceStart);

    m_chrono -= deltaTime;
}

void PlayState::Draw(double timeSinceStart, const GameContext& ctx) const
{
    ctx.terrain.Draw(timeSinceStart);

    for (const PlayerState& player : ctx.players)
        player.lasso.Draw();

    std::vector<DinoEntity*> entities;
    for (const PlayerState& player : ctx.players)
        entities.emplace_back(const_cast<DinoPlayer*>(&player.dino));
    for (const DinoAnimal& animal : m_animals)
        entities.emplace_back(const_cast<DinoAnimal*>(&animal));

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);
    for (DinoEntity* e : entities)
        e->Draw(timeSinceStart);

    DrawScoreNotifs(timeSinceStart);

    
    {
        std::string text = std::format("{:.2f}", m_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (ctx.renderSize.x - textSize.x * 2) / 2;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, 0}, 2);
    }

    if (m_bPause)
        DrawPauseMenu(ctx);
}

void PlayState::DrawScoreNotifs(double timeSinceStart) const
{
    for (const ScoreNotif& notif : m_scoreNotifs) {
        double timeAlive = timeSinceStart - notif.spawnTime;
        float posY = notif.position.y - (float)(timeAlive * 20.0);
        std::string text = std::format("+{}", notif.points);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, notif.color, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "notif");
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {notif.position.x, posY}, 2);
    }
}

void PlayState::DrawPauseMenu(const GameContext& ctx) const
{
    constexpr float SCALE_TITLE = 4.f;
    constexpr float SCALE_ITEM  = 2.f;
    constexpr float GAP         = 10.f;

    float currentY = ctx.renderSize.y * 0.15f;

    {
        std::vector<DinoVertex> vs;
        DinoVec2 sz = Dino_GenVertices_Text(vs, "-- PAUSE --", DinoColor_WHITE, DinoColor_BLACK);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Pause Title");
        float tx = (ctx.renderSize.x - sz.x * SCALE_TITLE) / 2.f;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, currentY}, SCALE_TITLE);
        currentY += sz.y * SCALE_TITLE + GAP * 2.f;
    }

    struct MenuItem { const char* label; int idx; };
    const MenuItem items[] = {
        { "RESET",     0 },
        { "LOBBY",     1 },
        { "CHRONO",    2 },
        { "REPRENDRE", 3 },
    };

    for (const MenuItem& item : items) {
        DinoColor color = (m_pauseMenuIdx == item.idx) ? DinoColor_YELLOW : DinoColor_WHITE;
        std::vector<DinoVertex> vs;
        DinoVec2 sz = Dino_GenVertices_Text(vs, item.label, color, DinoColor_BLACK);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), item.label);
        float tx = (ctx.renderSize.x - sz.x * SCALE_ITEM) / 2.f;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, currentY}, SCALE_ITEM);
        currentY += sz.y * SCALE_ITEM + GAP;
    }
}