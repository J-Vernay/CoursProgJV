/// @file dino_draw_utils.h
/// @brief Fonctions pour dessiner des primitives de rendu.
/// @author Julien Vernay

#pragma once

#include <dino/xdino.h>
#include <span>
#include <string_view>

/// @name Fonctions GenVertices
/// Ces fonctions génèrent des sommets dans 'out', pour être ensuite ajouté à des draw calls avec
/// des texIDs particuliers, et avec `translation`, `rotation` et `scale`.
/// @{

/// Produit un dessin de cercle avec la couleur spécifiée.
/// Les sommets doivent être dessiné avec XDino_TEXID_FONT.
///
/// @param out Destination dans laquelle sont ajoutés les sommets.
/// @param radius Rayon du cercle, en pixels.
/// @param color Couleur du cercle.
/// @param pos Position du coin supérieur gauche, en pixels.
void Dino_GenVertices_Circle(std::vector<DinoVertex>& out, float radius, DinoColor color = DinoColor_WHITE);

/// Produit un dessin contenant du texte, avec éventuellement une couleur de fond.
/// Les sommets doivent être dessiné avec XDino_TEXID_FONT.
///
/// @param out Destination dans laquelle sont ajoutés les sommets.
/// @param text Caractères à afficher.
/// @param color Couleur du texte.
/// @param colorBackground Couleur du rectangle affiché derrière le texte.
/// @param pos Position du coin supérieur gauche, en pixels.
/// @return La taille en pixels de la zone de texte.
DinoVec2 Dino_GenVertices_Text(
    std::vector<DinoVertex>& out, std::string_view text, DinoColor color = DinoColor_WHITE,
    DinoColor colorBackground = DinoColor_INVISIBLE, DinoVec2 pos = {0, 0}
);

/// Produit un dessin contenant une liste de segments, tous reliés.
/// Les sommets doivent être dessiné avec XDino_TEXID_WHITE.
///
/// @param out Destination dans laquelle sont ajoutés les sommets.
/// @param points Liste de points par lesquels la polyligne passe.
/// @param width Epaisseur du trait, en pixels.
/// @param color Couleur du trait.
void Dino_GenVertices_Polyline(
    std::vector<DinoVertex>& out, const std::vector<DinoVec2>& points, float width, DinoColor color = DinoColor_WHITE
);

// À bouger dans vos fichiers dino_animal.cpp/dino_animal.h

enum class EAnimalKind : int { Pig1, Pig2, Cow1, Cow2, Sheep1, Sheep2, Ostrich1, Ostrich2 };

enum class EAnimalAnim : int { Up, Down, Left, Right };

/// Produit une liste de sommets qui correspond à un animal animé.
///
/// @param out Destination dans laquelle sont ajoutés les sommets.
/// @param idxAnimal Quel espèce d'animal afficher, entre 0 et 7.
/// @param idxAnim Quelle animation jouer.
void Dino_GenVertices_Animal(
    std::vector<DinoVertex>& out, EAnimalKind kind, EAnimalAnim anim, double time);

/// @}

class DinoVertexBuffer {
public:
    DinoVertexBuffer(DinoVertex const* pVertices, size_t vertexCount, char const* pLabel);
    ~DinoVertexBuffer();

    // Constructeur par copie
    DinoVertexBuffer(DinoVertexBuffer const&) = delete;

    // Opérateur d'assignement par copie
    DinoVertexBuffer& operator=(DinoVertexBuffer const& other) = delete;

    uint64_t Get();

private:
    uint64_t m_vbufID;
};