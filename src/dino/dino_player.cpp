#include "dino_player.h"


double rotation = 360.0;
double scale = 1.0;
DinoVec2 dinoPos = {};
std::vector<DinoVec2> polyline;

// Constantes.
constexpr float speed = 300.f; // Nombre de pixels parcourus en une seconde.



void DinoPlayer::UpdatePlayer(float deltaTime)
{

    for (DinoGamepadIdx gamepadIdx : DinoGamepadIdx_ALL) {
        DinoGamepad gamepad{};
        bool bSuccess = XDino_GetGamepad(gamepadIdx, gamepad);
        if (!bSuccess)
            continue;

        if (gamepad.btn_down && !gamepad.btn_up)
            scale /= 1.01;
        if (gamepad.btn_up && !gamepad.btn_down)
            scale *= 1.01;
        if (gamepad.btn_left && !gamepad.btn_right)
            rotation += 90.0 * deltaTime;
        if (gamepad.btn_right && !gamepad.btn_left)
            rotation -= 90.0 * deltaTime;

        dinoPos.x += gamepad.stick_left_x * speed * deltaTime;
        dinoPos.y += gamepad.stick_left_y * speed * deltaTime;

        if (gamepad.stick_left_x != 0) {
            bMirror = gamepad.stick_left_x < 0;
        }

        bIdle = gamepad.stick_left_x == 0 && gamepad.stick_left_y == 0;

        if (!bIdle) {
            if (gamepad.btn_right) {
                bRunning = true;
                bWalking = false;
            }

            else {
                bRunning = false;
                bWalking = true;
            }
        }
        else {
            bRunning = false;
            bWalking = false;
        }
    }
}

void DinoPlayer::DrawPlayer(double timeSinceStart)
{

    // Dessin du dinosaur qu'on peut bouger
    {
        DinoDrawCall drawCall;
        drawCall.textureName = "dinosaurs.png";
        DinoVec2 posA = {0, 0};
        DinoVec2 posB = {24, 0};
        DinoVec2 posC = {0, 24};
        DinoVec2 posD = {24, 24};

        int u = 0;

        if (bIdle) {
            int idxFrame = int(timeSinceStart * 8) % 4;
            u = 0 + 24 * idxFrame;
        }
        if (bWalking) {
            int idxFrame = int(timeSinceStart * 8) % 6;
            u = 96 + 24 * idxFrame;
        }
        if (bRunning) {
            int idxFrame = int(timeSinceStart * 16) % 6;
            u = 432 + 24 * idxFrame;
        }

        switch (bMirror) {
        case false: drawCall.vertices.emplace_back(posA, 0 + u, 0);
            drawCall.vertices.emplace_back(posB, 24 + u, 0);
            drawCall.vertices.emplace_back(posC, 0 + u, 24);
            drawCall.vertices.emplace_back(posB, 24 + u, 0);
            drawCall.vertices.emplace_back(posC, 0 + u, 24);
            drawCall.vertices.emplace_back(posD, 24 + u, 24);
            break;
        case true: drawCall.vertices.emplace_back(posA, 24 + u, 0);
            drawCall.vertices.emplace_back(posB, 0 + u, 0);
            drawCall.vertices.emplace_back(posC, 24 + u, 24);
            drawCall.vertices.emplace_back(posB, 0 + u, 0);
            drawCall.vertices.emplace_back(posC, 24 + u, 24);
            drawCall.vertices.emplace_back(posD, 0 + u, 24);
            break;
        }

        drawCall.scale = scale;
        drawCall.translation = dinoPos;

        XDino_Draw(drawCall);

    }
}