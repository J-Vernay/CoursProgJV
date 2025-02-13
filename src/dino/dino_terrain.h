#pragma once

constexpr int TERRAIN_WIDTH = 16;
constexpr int TERRAIN_HEIGHT = 12;

class DinoTerrain {
    float timer = 0;
    int frame = 0;

public:
    void update(float deltaTime);
    void draw_ocean();
    void draw_terrain();
};