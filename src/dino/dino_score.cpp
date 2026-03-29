#include "dino_score.h"
#include <dino/dino_draw_utils.h>
#include <format>

std::vector<DinoNotification> DinoScoreManager::s_notifications;

void DinoScoreManager::AddNotification(DinoVec2 pos, int points, DinoColor color, double currentTime)
{
    s_notifications.push_back({
        pos,
        std::format("+{}", points),
        color,
        currentTime + 1.5
    });
}

void DinoScoreManager::DrawNotifications(double currentTime)
{
    auto itNotif = s_notifications.begin();
    while (itNotif != s_notifications.end()) {
        if (currentTime > itNotif->expirationTime) {
            itNotif = s_notifications.erase(itNotif);
        }
        else {
            std::vector<DinoVertex> vs;
            Dino_GenVertices_Text(vs, itNotif->text, itNotif->color, DinoColor_TRANSPARENT);
            DinoVertexBuffer vbuf(vs.data(), vs.size(), "ScoreNotif");

            float age = static_cast<float>(itNotif->expirationTime - currentTime);
            float yOffset = age * 15.0f;

            XDino_Draw(vbuf.Get(), XDino_TEXID_FONT, {itNotif->pos.x, itNotif->pos.y - 30 + yOffset}, 1);
            ++itNotif;
        }
    }
}