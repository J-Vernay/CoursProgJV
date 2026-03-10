#pragma once
#include <vector>
#include <dino/xdino.h>

struct Anim {
    std::vector<int> posU;
    int framesPerSecond;
};

class DinoPlayer {
public:
    DinoPlayer();

    void Init();
    void Update(double timeSinceStart, float deltaTime);
    void Draw(double timeSinceStart) const;
    void Shut();

    DinoVec2 GetPosition() const
    {
        return pos;
    }

    bool IsRunning() const
    {
        return isRunning;
    }

    bool IsGoingLeft() const
    {
        return isGoingLeft;
    }

    int currentV = 0;

private:
    DinoVec2 pos;
    bool isGoingLeft = false;
    bool isRunning = false;
    bool isHit = false;

    double hitEndTime = 0.0;

    uint64_t texID_dino = 0;

    Anim idleAnim;
    Anim walkAnim;
    Anim runAnim;
    Anim hitAnim;

    Anim* currentAnim = nullptr;
};