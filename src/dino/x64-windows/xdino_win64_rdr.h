/// @file xdino_win64_rdr.h
/// @brief Fonctions internes à XDino, pour le rendu graphique sur Windows.
/// Ces fonctions communiquent avec la carte graphique et sont par conséquent
/// complexes à comprendre. Elles ne seront pas étudiées dans ce cours.

#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>

/// @name API de rendus propre à la plateforme.
/// Ces fonctions communiquent avec la carte graphique et sont par conséquent
/// complexes à comprendre. Elles ne seront pas étudiées dans ce cours.
/// @{

/// Spécifique à la plateforme. Initialise le rendu.
void XDino_Win64_CreateRenderer(HWND hWindow, int32_t width, int32_t height);

/// Spécifique à la plateforme. Redimensionnne le rendu.
void XDino_Win64_ResizeRenderer(int32_t width, int32_t height);

/// Spécifique à la plateforme. Charge une texture.
void XDino_Win64_CreateTexture(std::string const& textureName);

/// Spécifique à la plateforme. Commence la passe de rendu.
void XDino_Win64_BeginDraw();

/// Spécifique à la plateforme. Finit la passe de rendu et envoit le travail à la carte graphique.
void XDino_Win64_EndDraw();

/// Spécifique à la plateforme. Libère les ressources associées au rendu.
void XDino_Win64_DestroyRenderer();

/// @}