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
/// Produit un dessin d'un sprite a partir d'un blit d'une texture
/// @param tex tecture complete
/// @param pixelSize pixel par unité
/// @param uBlit U de l'uv
/// @param vBlit V de l'uv
/// @param scale scale du sprite affiché
/// @param color couleur du sprite
/// @return 
DinoDrawCall Dino_CreateDrawCall_Sprite(const std::string& tex, uint16_t pixelSize, uint16_t uBlit, uint16_t vBlit, int scale, DinoColor color = DinoColor_WHITE);

/// Produit un dessin d'un sprite inversé par uv a partir d'un blit d'une texture
/// @param tex texture complete
/// @param pixelSize pixel par unité
/// @param uBlit U de l'uv
/// @param vBlit V de l'uv
/// @param scale scale du sprite affiché
/// @param color couleur du sprite
/// @return 
DinoDrawCall Dino_CreateDrawCall_InvertedSprite(const std::string& tex, uint16_t pixelSize, uint16_t uBlit, uint16_t vBlit, int scale, DinoColor color = DinoColor_WHITE);

/// @}
