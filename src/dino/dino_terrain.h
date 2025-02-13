#pragma once

constexpr int TERRAIN_WIDTH = 16;
constexpr int TERRAIN_HEIGHT = 12;

class DinoTerrain {
public:
    enum Season {
        SPRING,
        SUMMER,
        AUTUMN,
        WINTER
    };
    
private:
    float timer = 0;
    int frame = 0;
    Season season = SPRING;

    int16_t decorations[TERRAIN_WIDTH * TERRAIN_HEIGHT];

public:
    void init();
    void update(float deltaTime);
    void draw_ocean();
    void draw_terrain();
};