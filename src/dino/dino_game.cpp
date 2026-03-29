#include <dino/dino_draw_utils.h>
#include <dino/xdino.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>
#include <dino/dino_animal.h>
#include <dino/dino_lasso.h>
#include <dino/dino_tree.h>
#include <dino/dino_lobby.h>
#include <dino/dino_ingame.h>
#include <dino/dino_pause.h>
#include <dino/dino_game.h>
#include <dino/dino_score.h>


#include <format>
#include <algorithm>
#include <optional>

double g_lastTime = 0;
double g_timeSpawnAnimal = 0;
double g_chrono = 60;
bool g_bWasStartPressed = false;

constexpr DinoVec2 RENDER_SIZE = {480, 360};

std::vector<DinoGamepadIdx> g_UnassignedGamepads;
std::vector<PlayerState> g_Players;
std::vector<DinoTree> g_Trees;
std::vector<DinoAnimal> g_Animals;
DinoTerrain g_Terrain;
auto g_state = EGameState::Lobby;

std::optional<DinoVertexBuffer> g_vbufID_prenom;
DinoVec2 textSize_prenom;

void Dino_GameInit()
{
    XDino_SetRenderSize(RENDER_SIZE);
    DinoPlayer::InitStatic();
    DinoAnimal::InitStatic();
    DinoTree::InitStatic();

    for (DinoGamepadIdx idx : DinoGamepadIdx_ALL)
        g_UnassignedGamepads.emplace_back(idx);

    int idxSeason = XDino_RandomInt32(0, 3);
    g_Terrain.Init(RENDER_SIZE, idxSeason);

    {
        std::vector<DinoVertex> vs;
        textSize_prenom = Dino_GenVertices_Text(vs, "Mikha", DinoColor_WHITE, DinoColor_GREY);
        g_vbufID_prenom.emplace(vs.data(), vs.size(), "Prenom");
    }

    DinoVec2 terrainMin = g_Terrain.GetTopLeft();
    DinoVec2 terrainMax = g_Terrain.GetBottomRight();
    for (int i = 0; i < 4; ++i) {
        float x = terrainMin.x + (1 + i) * ((terrainMax.x - terrainMin.x) / 5);
        float y = terrainMin.y + 80;
        g_Trees.emplace_back(DinoVec2{x, y}, i);
    }
}

void Dino_GameFrame(double timeSinceStart)
{
    float deltaTime = static_cast<float>(timeSinceStart - g_lastTime);
    g_lastTime = timeSinceStart;
    XDino_SetRenderSize(RENDER_SIZE);

    // Lecture inputs
    bool bPressedStart = false;
    for (PlayerState& player : g_Players) {
        DinoGamepad gamepad;
        if (XDino_GetGamepad(player.gamepadIdx, gamepad)) {
            player.gamepad = gamepad;
            bPressedStart = bPressedStart || gamepad.start;
        }
    }

    // Construction entities
    std::vector<DinoEntity*> entities;
    for (PlayerState& player : g_Players)
        entities.emplace_back(&player.dino);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);
    if (g_state == EGameState::Lobby)
        for (DinoTree& tree : g_Trees)
            entities.emplace_back(&tree);

    // Update selon état
    switch (g_state) {
    case EGameState::Lobby: Lobby_Update(timeSinceStart,
                                         deltaTime,
                                         g_Players,
                                         g_UnassignedGamepads,
                                         g_Trees,
                                         entities,
                                         g_Terrain,
                                         g_state);
        break;
    case EGameState::InGame: InGame_Update(timeSinceStart,
                                           deltaTime,
                                           g_Players,
                                           g_Animals,
                                           entities,
                                           g_Terrain,
                                           g_chrono,
                                           g_timeSpawnAnimal,
                                           bPressedStart,
                                           g_bWasStartPressed,
                                           g_state);
        break;
    case EGameState::Paused: Pause_Update(bPressedStart, g_bWasStartPressed, g_state);
        break;
    }

    // Reconstruire entities 
    entities.clear();
    for (PlayerState& player : g_Players)
        entities.emplace_back(&player.dino);
    for (DinoAnimal& animal : g_Animals)
        entities.emplace_back(&animal);
    if (g_state == EGameState::Lobby)
        for (DinoTree& tree : g_Trees)
            entities.emplace_back(&tree);

    std::sort(entities.begin(), entities.end(), DinoEntity::CompareVerticalPos);

    // Affichage commun
    constexpr DinoColor CLEAR_COLOR = {50, 50, 80, 255};
    XDino_SetClearColor(CLEAR_COLOR);
    g_Terrain.Draw();

    switch (g_state) {
    case EGameState::Lobby: Lobby_Draw(timeSinceStart, g_Players, entities);
        Score_Draw(g_Players, timeSinceStart);
        break;
    case EGameState::InGame: InGame_Draw(timeSinceStart, g_Players, entities);
        Score_Draw(g_Players, timeSinceStart);
        break;
    case EGameState::Paused: InGame_Draw(timeSinceStart, g_Players, entities);
        Pause_Draw(timeSinceStart);
        Score_Draw(g_Players, timeSinceStart);
        break;
    }

    // dTime
    {
        std::string text = std::format("dTime={:04.1f}ms", deltaTime * 1000.0);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_GREY);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "dTime");
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {}, 2);
    }

    // Chrono
    {
        std::string text = std::format("{:.2f}", g_chrono);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, DinoColor_WHITE, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Chrono");
        float tx = (RENDER_SIZE.x - textSize.x * 2) / 2;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, 0}, 2);
    }

    // Prénom
    {
        float tx = RENDER_SIZE.x - textSize_prenom.x * 2;
        float ty = RENDER_SIZE.y - textSize_prenom.y * 2;
        XDino_Draw(g_vbufID_prenom->Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

#if !XDINO_RELEASE
    DinoGamepad keyboard;
    bool bKeyboardOk = XDino_GetGamepad(DinoGamepadIdx::Keyboard, keyboard);
    if (bKeyboardOk && keyboard.shoulder_left) {
        int diff = keyboard.dpad_up ? -1 : keyboard.dpad_down ? 1 : 0;
        XDino_DrawStats(diff);
    }
#endif
}

void Dino_GameShut()
{
    for (PlayerState& player : g_Players)
        player.dino.Shut();
    for (DinoAnimal& animal : g_Animals)
        animal.Shut();
    g_Terrain.Shut();

    DinoTree::ShutStatic();
    DinoPlayer::ShutStatic();
    DinoAnimal::ShutStatic();
    g_vbufID_prenom.reset();
}