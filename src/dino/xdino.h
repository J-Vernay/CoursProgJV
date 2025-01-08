/// @file xdino.h
/// @brief API d'interaction entre le moteur de jeu "XDino" et la logique de jeu.
/// @author Julien Vernay

#pragma once

#include <stdint.h>

#include <vector>
#include <string>

/// À appeler en cas d'erreur critique (= affiche une popup et ferme le jeu)
#define DINO_CRITICAL(msg) (_impl_XDino_Critical(__func__, __LINE__, msg), __debugbreak())

/// @name Fonctions qui doivent être implémentées par la logique de jeu.
/// @{

/// Appelé au tout début, pour initialiser la logique de jeu.
void Dino_GameInit();

/// Appelé pour générer chaque frame. Doit lireles entrées de jeu
/// avec `XDino_GetGamepad()`, mettre à jour la logique du jeu,
/// et donner les commandes de dessins avec `XDino_Draw()`.
/// @param timeSinceStart Temps en secondes depuis le lancement du jeu.
void Dino_GameFrame(double timeSinceStart);

/// Appelé quand l'utilisateur demande à quitter le jeu (touche ECHAP).
void Dino_GameShut();

/// @}

/// @name API graphique, à appeler à l'intérieur de `Dino_GameDraw()`.
/// @{

/// Représente une coordonnée 2D en pixels.
struct DinoVec2 {
    float x;
    float y;
};

/// Représente une couleur de pixels, utilisée pour moduler l'affichage d'une texture.
union DinoColor {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    uint32_t rgba;
};

constexpr DinoColor DinoColor_WHITE{0xFF, 0xFF, 0xFF, 0xFF};
constexpr DinoColor DinoColor_GREY{0x80, 0x80, 0x80, 0xFF};
constexpr DinoColor DinoColor_BLACK{0x00, 0x00, 0x00, 0xFF};
constexpr DinoColor DinoColor_TRANSPARENT{0xFF, 0xFF, 0xFF, 0x80};
constexpr DinoColor DinoColor_INVISIBLE{0xFF, 0xFF, 0xFF, 0x00};

/// Représente un des sommets d'un triangle envoyé à la carte graphique.
struct DinoVertex {
    /// Position à l'écran (décalée suivant `XDino_SetTransform()`)
    DinoVec2 pos{};
    /// Coordonnée X du pixel correspondant dans la texture mise avec `XDino_BindTexture()`.
    uint16_t u = 0;
    /// Coordonnée Y du pixel correspondant dans la texture mise avec `XDino_BindTexture()`.
    uint16_t v = 0;
    /// Couleur utilisée pour moduler la texture (par défaut, pas de modulation).
    DinoColor color = DinoColor_WHITE;
};

/// Représente une demande de dessin à la carte graphique.
struct DinoDrawCall {
    /// Sommets des triangles à envoyer à la carte graphique.
    std::vector<DinoVertex> vertices;
    /// Nom de la texture à utiliser, ou vide pour une texture complètement blanche et opaque.
    std::string textureName;
    /// Décalage en pixels depuis l'angle en haut à gauche.
    DinoVec2 translation = DinoVec2{0.0f, 0.0f};
    /// Rotation en degrés.
    double rotation = 0.0;
    /// Mise à l'échelle.
    double scale = 1.0;
};

/// Taille de la fenêtre de rendu.
DinoVec2 XDino_GetWindowSize();

/// Pour indiquer la couleur d'arrière-plan.
void XDino_SetClearColor(DinoColor color);

/// Transfère les triangles à dessiner à la carte graphique.
void XDino_Draw(DinoDrawCall drawCall);

/// @}

/// @name API clavier/souris/manette, à appeler à l'intérieur de `Dino_GameUpdate()`.
/// @{

enum class DinoGamepadIdx : int32_t {
    Gamepad1,
    Gamepad2,
    Gamepad3,
    Gamepad4,
    Keyboard,
};

constexpr DinoGamepadIdx DinoGamepadIdx_ALL[] = {
    DinoGamepadIdx::Gamepad1,
    DinoGamepadIdx::Gamepad2,
    DinoGamepadIdx::Gamepad3,
    DinoGamepadIdx::Gamepad4,
    DinoGamepadIdx::Keyboard,
};

/// Structure contenant l'état d'une manette (ou du clavier utilisé comme manette).
struct DinoGamepad {
    bool dpad_up : 1; ///< Si clavier : Flèche du haut
    bool dpad_left : 1; ///< Si clavier : Flèche gauche
    bool dpad_right : 1; ///< Si clavier : Flèche droite
    bool dpad_down : 1; ///< Si clavier : Flèche du bas
    bool btn_up : 1; ///< Si clavier : Z ou W
    bool btn_left : 1; ///< Si clavier : Q ou A
    bool btn_right : 1; ///< Si clavier : D
    bool btn_down : 1; ///< Si clavier : S
    bool start : 1; ///< Si clavier : ESPACE ou ENTREE
    bool select : 1; ///< Si clavier : CTRL ou SHIFT
    bool shoulder_left : 1; ///< Si clavier : Clic gauche
    bool shoulder_right : 1; ///< Si clavier : Clic droit

    float stick_left_x; ///< Entre -1 et 1. Si clavier : dérivé de 'dpad'
    float stick_left_y; ///< Entre -1 et 1. Si clavier : dérivdé de 'dpad'
    float stick_right_x; ///< Entre -1 et 1. Si clavier : dérivé de 'btn'
    float stick_right_y; ///< Entre -1 et 1. Si clavier : dérivé de 'btn'

    int32_t mouse_x; ///< Si clavier : position en pixels.
    int32_t mouse_y; ///< Si clavier : position en pixels.
};

/// Récupère l'état de la manette concernée.
/// @param idx Quelle manette regarder, ou le clavier avec `DinoGamepadIdx::Keyboard`.
/// @param outGamepad Où sont stockées les valeurs récupérées.
/// @return `true` quand réussit, `false` si non-disponible.
bool XDino_GetGamepad(DinoGamepadIdx idx, DinoGamepad& outGamepad);

/// @}


/// Détail d'implémentation, ne pas appeler directement. Il faut utiliser `DINO_CRITICAL`.
void _impl_XDino_Critical(char const* pFunc, int line, char const* msg);