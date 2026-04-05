#include <dino/dino_score.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_game.h>
#include <format>
#include <algorithm>

constexpr DinoColor SCORE_COLORS[4] = {
    DinoColor_BLUE, DinoColor_RED, DinoColor_YELLOW, DinoColor_GREEN
};

constexpr double NOTIFICATION_DURATION = 1.0;

static std::vector<ScoreNotification> g_notifications;

void Score_AddNotification(DinoVec2 pos, int points, DinoColor color, double timeSinceStart)
{
    g_notifications.push_back({pos, points, color, timeSinceStart});
}

void Score_Draw(std::vector<PlayerState>& players, double timeSinceStart)
{
    // Affiche les scores
    constexpr DinoVec2 RENDER_SIZE = {480, 360};

    for (int i = 0; i < static_cast<int>(players.size()); ++i) {
        std::string text = std::format("P{}: {}", i + 1, players[i].score);
        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, SCORE_COLORS[i], DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Score");
        float tx = 0;
        float ty = RENDER_SIZE.y / 2 - (players.size() * textSize.y * 2) / 2 + i * textSize.y * 2;
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {tx, ty}, 2);
    }

    // Purge les notifications expirées
    auto it = std::remove_if(g_notifications.begin(),
                             g_notifications.end(),
                             [timeSinceStart](const ScoreNotification& n) {
                                 return timeSinceStart - n.spawnTime > NOTIFICATION_DURATION;
                             });
    g_notifications.erase(it, g_notifications.end());

    // Affiche les notifications
    for (ScoreNotification& n : g_notifications) {
        std::string text = std::format("+{}", n.points);
        std::vector<DinoVertex> vs;
        Dino_GenVertices_Text(vs, text, n.color, DinoColor_TRANSPARENT);
        DinoVertexBuffer vbuf(vs.data(), vs.size(), "Notif");
        // Monte vers le haut au fil du temps
        float elapsed = static_cast<float>(timeSinceStart - n.spawnTime);
        DinoVec2 drawPos = {n.pos.x, n.pos.y - elapsed * 30};
        XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, drawPos, 2);
    }
}