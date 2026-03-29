#include "dino_score.h"
#include <dino/dino_draw_utils.h>
#include <format>

// Initialisation du membre statique
std::vector<DinoNotification> DinoScoreManager::s_notifications;

void DinoScoreManager::AddNotification(DinoVec2 pos, int points, DinoColor color, double currentTime)
{
    s_notifications.push_back({
        pos,
        std::format("+{}", points),
        color,
        currentTime + 1.5 // Durée de vie de 1.5s
    });
}

void DinoScoreManager::DrawNotifications(double currentTime)
{
    auto it = s_notifications.begin();
    while (it != s_notifications.end()) {
        if (currentTime > it->expirationTime) {
            it = s_notifications.erase(it);
        }
        else {
            std::vector<DinoVertex> vs;
            Dino_GenVertices_Text(vs, it->text, it->color, DinoColor_TRANSPARENT);
            DinoVertexBuffer vbuf(vs.data(), vs.size(), "ScoreNotif");

            // Petit effet de montée fluide
            float age = static_cast<float>(it->expirationTime - currentTime);
            float yOffset = age * 15.0f;

            XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {it->pos.x, it->pos.y - 30 + yOffset}, 1);
            ++it;
        }
    }
}