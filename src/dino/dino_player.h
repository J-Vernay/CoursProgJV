#pragma once
#include <vector>
#include <dino/xdino.h>

struct Anim {
    std::vector<int> posU;
    int framesPerSecond;
};

class DinoTerrain;

class DinoPlayer {
public:
    DinoPlayer();

    void Init(const DinoTerrain& terrain, int colorV);
    void Update(double timeSinceStart, float deltaTime, const DinoTerrain& terrain);
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

private:
    DinoVec2 pos;
    float baseSpeed = 100.f;
    bool isGoingLeft = false;
    bool isRunning = false;
    bool isHit = false;

    double hitEndTime = 0.0;
    int currentV = 0;

    uint64_t texID_dino = 0;

    Anim idleAnim;
    Anim walkAnim;
    Anim runAnim;
    Anim hitAnim;

    Anim* currentAnim = nullptr;
};