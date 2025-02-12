/// @file dino_draw_utils.h
/// @brief Fonctions pour dessiner des primitives de rendu.
/// @author Julien Vernay

#pragma once

#include <dino/xdino.h>
#include <string_view>
#include <span>

/// @name Fonctions CreateDrawCall
/// Ces fonctions créent des `DinoDrawCall` avec `vertices` et `textureName`.
/// On peut ensuite modifier `translation`, `rotation` et `scale`.
/// @{

/// Produit un dessin de cercle avec la couleur spécifiée.
/// La position (0,0) correspond au centre du cercle, qu'il faut décaler grâce à `translation`.
/// @param radius Rayon du cercle, en pixels.
/// @param color Couleur du cercle.
DinoDrawCall Dino_CreateDrawCall_Circle(float radius, DinoColor color = DinoColor_WHITE);

/// Produit un dessin contenant du texte, avec éventuellement une couleur de fond.
/// La position (0,0) correspond au coin en haut à gauche, qu'il faut décaler grâce à `translation`.
/// @param text Caractères à afficher.
/// @param color Couleur du texte.
/// @param colorBackground Couleur du rectangle affiché derrière le texte.
/// @param pOutSize Si non-NULL, récupère la taille en pixels du texte.
DinoDrawCall Dino_CreateDrawCall_Text(std::string_view text, DinoColor color = DinoColor_WHITE,
                                      DinoColor colorBackground = DinoColor_INVISIBLE, DinoVec2* pOutSize = nullptr);

/// Produit un dessin contenant une liste de segments, tous reliés.
/// @param points Liste de points par lesquels la polyligne passe.
/// @param width Epaisseur du trait, en pixels.
/// @param color Couleur du trait.
DinoDrawCall Dino_CreateDrawCall_Polyline(std::span<DinoVec2 const> points, float width,
                                          DinoColor color = DinoColor_WHITE);

/// Produit un dessin contenant une sprite depuis une spritesheet.
/// La position (0,0) correspond au coin en haut à gauche, qu'il faut décaler grâce à `translation`.
/// Il faut indiquer la texture à utiliser grâce à `textureName`.
/// @param u Position U en haut à gauche de la sprite, en pixels.
/// @param v Position V en haut à gauche de la sprite, en pixels.
/// @param width Largeur de la sprite, en pixels.
/// @param height Hauteur de la sprite, en pixels.
/// @param flip Si `true` la sprite sera inversé horizontalement.
DinoDrawCall Dino_CreateDrawCall_Sprite(int16_t u, int16_t v, int16_t width, int16_t height, bool flip = false);

/// @}
