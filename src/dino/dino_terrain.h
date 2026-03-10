#pragma once
#include <vector>
#include <dino/xdino.h>

struct Flower {
    DinoVec2 pos;
    int type; // 0, 1 ou 2 correspondant aux 3 fleurs
};

class DinoTerrain {
public:
    void Init();
    void Draw(double timeSinceStart);
    void Shut();

private:
    uint64_t texID_terrain = 0;

    int season = 0; // 0...3, correspond à U += (0,80,160,240)
    DinoVec2 terrainOrigin = {112, 84}; // Centre dans 480x360

    std::vector<Flower> flowers;

    void DrawOcean() const;
    void DrawGround() const;
    void DrawFlowers() const;
};