/// @file dino_animation.h
/// @brief Structures liées à l'animation d'objets.

#pragma once
#include <dino/xdino.h>

/// Représentation d'une animation
struct Animation {
    float framerate;
    std::vector<int16_t> frames;
};

/// Representation de l'état d'un animateur
template <typename T>
struct AnimatorState {
    int frame;
    float timer;
    T animationId;
};