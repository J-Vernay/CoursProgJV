#include "dino_player.h"

#include "dino_draw_utils.h"
#include "dino_geometry.h"

// Constantes.
constexpr float PLAYER_SPEED = 150.f; // Nombre de pixels parcourus en une seconde.

void DinoPlayer::Update(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.

    d_idle_ = false;
    d_walking_ = false;
    d_running_ = false;

    DinoGamepad gamepad;
    bool bSuccess = XDino_GetGamepad(idx_gamepad_, gamepad);
    if (!bSuccess)
        gamepad = {}; // Laisser vide, on considère le joueur immobile

    //Movement
    float speed = PLAYER_SPEED;
    XDino_ProfileBegin(DinoColor_GREY, "Input");
    if (gamepad.btn_right) 

        speed *= 2;
        
    pos_.x += gamepad.stick_left_x * speed * deltaTime;
    pos_.y += gamepad.stick_left_y * speed * deltaTime;
    XDino_ProfileEnd();
    
    if (gamepad.stick_left_x != 0)
        mirror_ = gamepad.stick_left_x < 0;
        
    d_idle_ = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
    if (!d_idle_) {
        if (gamepad.btn_right)
            d_running_ = true;
        else
            d_walking_ = true;
    }
    lasso.emplace_back(pos_);
    if (lasso.size() > 4) {
        DinoVec2 c = lasso[lasso.size() - 2];
        DinoVec2 d = lasso[lasso.size() - 1];

        for (int idxSegement1 = 0; idxSegement1 < lasso.size() - 3; idxSegement1++)
        {
            DinoVec2 a = lasso[idxSegement1];
            DinoVec2 b = lasso[idxSegement1 + 1];

            if (Dino_IntersectSegment(a, b, c, d))
            {
                lasso.erase(lasso.begin() + idxSegement1 + 1, lasso.end() - 1);
            }
        }
    }
}

void DinoPlayer::Display(double timeSinceStart)
{
    //Afficher le dinosaure
    {
        sprite_idx_ = static_cast<int>(timeSinceStart / 0.12);

XDino_ProfileBegin(DinoColor_GREEN, "Drawcall");
        
        DinoDrawCall drawCall;
        drawCall.textureName = "dinosaurs.png";
        DinoVec2 posA = {0,0};
        DinoVec2 posB = {24, 0};
        DinoVec2 posC = {0,24};
        DinoVec2 posD = {24,24};

        int pos = 0;
        if (d_idle_)
            pos = 0;
        if (d_walking_)
            pos = 96;
        if (d_running_)
            pos = 432;
        pos += sprite_idx_ % 4 * 24;
        
        if (mirror_)
        {
            drawCall.vertices.emplace_back(posA, pos + 24, idx_player_ * 24);
            drawCall.vertices.emplace_back(posB, pos, idx_player_ * 24);
            drawCall.vertices.emplace_back(posC, pos + 24, 24 + idx_player_ * 24);
            drawCall.vertices.emplace_back(posB, pos, idx_player_ * 24);
            drawCall.vertices.emplace_back(posC, pos + 24, 24 + idx_player_ * 24);
            drawCall.vertices.emplace_back(posD, pos, 24 + idx_player_ * 24);
        }
        else
        {
            drawCall.vertices.emplace_back(posA, pos, idx_player_ * 24);
            drawCall.vertices.emplace_back(posB, pos + 24, idx_player_ * 24);
            drawCall.vertices.emplace_back(posC, pos, 24 + idx_player_ * 24);
            drawCall.vertices.emplace_back(posB, pos + 24, idx_player_ * 24);
            drawCall.vertices.emplace_back(posC, pos, 24 + idx_player_ * 24);
            drawCall.vertices.emplace_back(posD, pos + 24, 24 + idx_player_ * 24);
        }

        DinoVec2 offset = {12, 20};
        DinoVec2 newPos = {pos_.x - offset.x, pos_.y - offset.y};
        drawCall.translation = newPos;
        XDino_Draw(drawCall);
        XDino_ProfileEnd();
    }
}

void DinoPlayer::DrawLasso()
{
    XDino_ProfileBegin(DinoColor_WHITE,"Draw Lasso") ;
    //Lasso
    DinoDrawCall lassoDrawcall = Dino_CreateDrawCall_Polyline(lasso, 4, lassoColor_);
    XDino_Draw(lassoDrawcall);
    XDino_ProfileEnd();
}

void DinoPlayer::DinoPlayerInit(DinoVec2 pos, int idx, DinoGamepadIdx gamepadIdx, DinoColor lassoColor)
{
    pos_ = pos;
    idx_player_ = idx;
    idx_gamepad_ = gamepadIdx;
    lassoColor_ = lassoColor;
}


void DinoPlayer::SetPlayerIdx(int idx)
{
    idx_player_ = idx;
}


