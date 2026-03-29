#pragma once
#include <dino/xdino.h>
#include <string>
#include <vector>

struct DinoNotification {
    DinoVec2 pos;
    std::string text;
    DinoColor color;
    double expirationTime;
};

class DinoScoreManager {
public:
    static void AddNotification(DinoVec2 pos, int points, DinoColor color, double currentTime);
    static void DrawNotifications(double currentTime);

private:
    static std::vector<DinoNotification> s_notifications;
};