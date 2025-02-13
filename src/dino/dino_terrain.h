/// @file dino_terrain.h
/// @brief Classe permettant de générer et dessiner un terrain à l'écran

#pragma once

constexpr int TERRAIN_WIDTH = 16;
constexpr int TERRAIN_HEIGHT = 12;

/// Terrain de jeu
class DinoTerrain {
public:
    /// Représentation de la saison du terrain
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
    void init(); ///< Initialise le terrain, choisis la saison et positionne les décorations.
    void update(float deltaTime); ///< Execute la logique d'animation du terrain.
    void draw_ocean(); ///< Dessine l'océan à l'écran.
    void draw_terrain(); ///< Dessine le terrain à l'écran.
    DinoVec2 get_terrain_min_position(); ///< Retourne la position à l'écran du coin en haut à gauche du terrain.
    DinoVec2 get_terrain_max_position(); ///< Retourne la position à l'écran du coin en bas à droite du terrain.
};