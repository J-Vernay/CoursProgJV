#pragma once
#include <dino/xdino.h>
#include <vector>

enum DinoState {
    IDLE = 0, WALK, RUN, DAMAGE, COUNT
};

struct AnimRange {
    int start;
    int count;
    double speed;
};

struct Player {
    DinoVec2 pos = {};
    DinoState state = IDLE;
    bool lookLeft = false;

    // Animation
    int startFrame = 0;
    int numberOfFrames = 2;
    double animationSpeed = 4;
    double currentFrame = 0;
    int currentImageIndex = 0;

    // Fonctions membres
    void Init(DinoVec2 startPos);
    void Update(float deltaTime);
    void SetState(DinoState newState);
    void Draw(uint64_t texID_dino);
};

// On déclare une instance globale pour y accéder partout si besoin
extern Player g_player;