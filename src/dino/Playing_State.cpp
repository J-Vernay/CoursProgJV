#include "Lobby_state.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <dino/Dino_GameStates.h>
#include <dino/Playing_state.h>
#include <stdio.h>
#include <string>

PlayState::PlayState(DinoGameState* dinoGameState, int season)
{
    m_season = season;
    m_dinoGameState = dinoGameState;
}

void PlayState::EnterState(double timeSinceStart)
{
    std::cout << "PlayingState::EnterState" << std::endl;
    m_dinoGameState->gamePreviouslyEnded = false;
    m_bWasStartPressed = true;
    m_terrain.Init(DinoGameState::RENDER_SIZE, m_season);
    m_dinoGameState->g_spawner.Init(m_dinoGameState->g_scoreManager);
    m_chrono = 60.0;
    m_paused = false;
}

void PlayState::UpdateState(float deltaTime, double timeSinceStart)
{
    std::cout << "PlayingState::UpdateState" << std::endl;
    bool bPressedStart = false;
    bool bPressedBtnRight = false;
    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad)) {
            player.gamepad = gamepad;
            bPressedStart = bPressedStart || gamepad.start;
            bPressedBtnRight = bPressedBtnRight || gamepad.btn_right;
        }
    }
    if (bPressedStart && !m_bWasStartPressed) {
        if (!m_paused) {
            m_paused = true;
            m_currentPauseButton = 3;
        }
    }
    m_bWasStartPressed = bPressedStart;

    if (bPressedBtnRight) {
        switch (m_currentPauseButton) {
        case 0: // Restart
            m_dinoGameState->g_scoreManager.ResetScores();
            m_paused = false;
            m_dinoGameState->ChangeState(
                std::make_unique<PlayState>(m_dinoGameState, m_season),
                timeSinceStart

            );
            m_paused = false;
            return;
        case 1: // Lobby
            m_dinoGameState->g_scoreManager.ResetScores();
            m_dinoGameState->ChangeState(
                std::make_unique<LobbyState>(m_dinoGameState, m_season),
                timeSinceStart
            );
            return;
        case 2: // Chrono 
            break;
        case 3: // Resume
            m_paused = false;
            break;
        }
    }
    if (m_paused) {
        UpdatePauseInput();
        return;
    }

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        player.player.Update(timeSinceStart, deltaTime, m_terrain, player.gamepad);

    auto& animals = m_dinoGameState->g_spawner.m_animals;
    auto it = std::remove_if(animals.begin(), animals.end(), DinoAnimal::IsDead);
    for (auto it2 = it; it2 < animals.end(); ++it2)
        it2->Shut();
    animals.erase(it, animals.end());
    m_dinoGameState->g_spawner.Update(deltaTime, timeSinceStart, m_chrono);

    m_entities.clear();
    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        m_entities.push_back(&player.player);
    for (DinoAnimal& animal : animals)
        m_entities.push_back(&animal);

    for (size_t idxA = 0; idxA < m_entities.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < m_entities.size(); ++idxB)
            DinoEntity::ResolveCollision(*m_entities[idxA], *m_entities[idxB]);

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        player.lasso.Update(player.player.GetPos());

    for (size_t idxA = 0; idxA < m_dinoGameState->g_players.size(); ++idxA)
        for (size_t idxB = idxA + 1; idxB < m_dinoGameState->g_players.size(); ++idxB)
            DinoLasso::ResolveCollision(m_dinoGameState->g_players[idxA].lasso,
                                        m_dinoGameState->g_players[idxB].lasso);

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players)
        for (DinoEntity* pEntity : m_entities)
            if (player.lasso.WasInLoop(pEntity->GetPos()))
                pEntity->ReactLoop(timeSinceStart, player.lasso.m_ownerIndex);

    m_dinoGameState->g_scoreManager.EndAnimalBonus();

    m_chrono -= deltaTime;
    if (m_chrono <= 0) {
        m_dinoGameState->ChangeState(
            std::make_unique<LobbyState>(m_dinoGameState, m_season),
            timeSinceStart
        );
        m_dinoGameState->gamePreviouslyEnded = true;
        return;
    }

    m_terrain.Update(timeSinceStart);
}

void PlayState::UpdatePauseInput()
{
    bool bUp = false, bDown = false, bLeft = false, bRight = false;
    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad)) {
            bUp = bUp || gamepad.dpad_up;
            bDown = bDown || gamepad.dpad_down;
            bLeft = bLeft || gamepad.dpad_left;
            bRight = bRight || gamepad.dpad_right;
        }
    }

    if (bUp && !m_bWasPauseUpPressed)
        m_currentPauseButton = (m_currentPauseButton + 3) % 4;
    if (bDown && !m_bWasPauseDownPressed)
        m_currentPauseButton = (m_currentPauseButton + 1) % 4;

    if (m_currentPauseButton == 2) {
        if (bLeft && !m_bWasPauseLeftPressed)
            m_chrono = std::max(10.0, m_chrono - 10.0);
        if (bRight && !m_bWasPauseRightPressed)
            m_chrono = std::min(60.0, m_chrono + 10.0);
    }

    m_bWasPauseUpPressed = bUp;
    m_bWasPauseDownPressed = bDown;
    m_bWasPauseLeftPressed = bLeft;
    m_bWasPauseRightPressed = bRight;
}

void PlayState::DrawState(float deltaTime, double timeSinceStart)
{
    std::cout << "PlayingState::DrawState" << std::endl;
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);
    XDino_SetRenderSize(DinoGameState::RENDER_SIZE);

    m_terrain.Draw(timeSinceStart);

    for (DinoGameState::PlayerState& player : m_dinoGameState->g_players
    )
        player.lasso.Draw();

    std::string text = std::format("{:2.2f}", m_chrono);
    std::vector<DinoVertex> vs;
    DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
    DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
    float tx = (DinoGameState::RENDER_SIZE.x - textSize.x * 2) / 2;
    XDino_Draw(vbuf.GetVbufID(), XDino_TEXID_FONT, {tx, 0}, 2);

    DinoVec2 renderSize = XDino_GetRenderSize();
    XDino_Draw(m_dinoGameState->m_vbuffID_nom,
               XDino_TEXID_FONT,
               {renderSize.x - m_dinoGameState->m_textSize_nom.x * 2,
                renderSize.y - m_dinoGameState->m_textSize_nom.y * 2},
               2);

    m_dinoGameState->g_scoreManager.DrawScores(m_terrain);

    std::sort(m_entities.begin(), m_entities.end(), DinoEntity::CompareVerticalPos);
    for (DinoEntity* pEntity : m_entities) {
        pEntity->Draw(timeSinceStart);
    }

    if (m_paused)
        DrawPauseMenu();
}

void PlayState::DrawPauseMenu()
{
    constexpr DinoVec2 CENTER = {240, 180};

    std::vector<DinoVertex> vs;

    // Title
    DinoVec2 ts = Dino_GenVertices_Text(vs, "-- PAUSE --", DinoColor_WHITE, DinoColor_BLACK);
    DinoVertexBuffer vbuf(vs.data(), vs.size(), "PauseTitle");
    XDino_Draw(vbuf.GetVbufID(),
               XDino_TEXID_FONT,
               {(DinoGameState::RENDER_SIZE.x - ts.x * 4) / 2,
                (DinoGameState::RENDER_SIZE.y - ts.y * 20) / 2},
               4);

    struct Entry {
        const char* label;
        float y;
        int idx;
    };
    constexpr Entry entries[] = {
        {"Recommencer", 150, 0},
        {"Lobby", 180, 1},
        {"Chrono", 210, 2},
        {"Reprendre", 240, 3},
    };

    for (const Entry& e : entries) {
        vs.clear();
        DinoVec2 entrySize = Dino_GenVertices_Text(vs,
                                                   e.label,
                                                   DinoColor_WHITE,
                                                   m_currentPauseButton == e.idx
                                                       ? DinoColor_GREY
                                                       : DinoColor_BLACK);
        DinoVertexBuffer vbufEntry(vs.data(), vs.size(), e.label);
        XDino_Draw(vbufEntry.GetVbufID(),
                   XDino_TEXID_FONT,
                   DinoVec2{240, e.y} - entrySize,
                   2);
    }

    if (m_currentPauseButton == 2) {
        vs.clear();
        std::string chronoStr = std::format("{:.0f}s", m_chrono);
        DinoVec2 chronoSize = Dino_GenVertices_Text(vs, chronoStr, DinoColor_YELLOW, DinoColor_BLACK);
        DinoVertexBuffer vbufChrono(vs.data(), vs.size(), "ChronoVal");
        XDino_Draw(vbufChrono.GetVbufID(),
                   XDino_TEXID_FONT,
                   DinoVec2{320, 210} - chronoSize,
                   2);
    }
}

void PlayState::ExitState()
{
    m_dinoGameState->g_spawner.Shut();
    m_terrain.Shut();
    m_entities.clear();
}