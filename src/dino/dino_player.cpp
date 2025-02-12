#include "dino_player.h"

// Constantes.
constexpr float PLAYER_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.

void DinoPlayer::UpdatePlayer(float deltaTime)
{
    // Gestion des entrées et mise à jour de la logique de jeu.

    d_idle_ = false;
    d_walking_ = false;
    d_running_ = false;
    
    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        //Movement
        float speed = PLAYER_SPEED;
        if (gamepad.btn_right) 
            speed *= 2;
        
        player_pos_.x += gamepad.stick_left_x * speed * deltaTime;
        player_pos_.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x != 0)
            mirror_ = gamepad.stick_left_x < 0;
        
        d_idle_ = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;
        if (!d_idle_) {
            if (gamepad.btn_right)
                d_running_ = true;
            else
                d_walking_ = true;
        }
    }
}

void DinoPlayer::DisplayPlayer(double timeSinceStart)
{
    //Afficher le dinosaure
    {
        spriteIdx = static_cast<int>(timeSinceStart / 0.12);
        
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
        pos += spriteIdx % 4 * 24;
        
        if (mirror_)
        {
            drawCall.vertices.emplace_back(posA, pos + 24, 0);
            drawCall.vertices.emplace_back(posB, pos, 0);
            drawCall.vertices.emplace_back(posC, pos + 24, 24);
            drawCall.vertices.emplace_back(posB, pos, 0);
            drawCall.vertices.emplace_back(posC, pos + 24, 24);
            drawCall.vertices.emplace_back(posD, pos, 24);
        }
        else
        {
            drawCall.vertices.emplace_back(posA, pos, 0);
            drawCall.vertices.emplace_back(posB, pos + 24, 0);
            drawCall.vertices.emplace_back(posC, pos, 24);
            drawCall.vertices.emplace_back(posB, pos + 24, 0);
            drawCall.vertices.emplace_back(posC, pos, 24);
            drawCall.vertices.emplace_back(posD, pos + 24, 24);
        }

        drawCall.scale = 2;
        drawCall.translation = player_pos_;
        XDino_Draw(drawCall);
    }
}

void DinoPlayer::SetPlayerPos(DinoVec2 pos)
{
    player_pos_ = pos;
}
