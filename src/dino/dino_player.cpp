#include <dino/dino_player.h>


void DinoPlayer::Init(DinoVec2 initPos, DinoGamepadIdx idxGamepad_, int32_t idxPlayer_)
{
    dinoPos = initPos;
    idxGamepad = idxGamepad_;
    idxPlayer = idxPlayer_;
    if (idxPlayer < 0 || idxPlayer > 4)
        DINO_CRITICAL("Only players indices [0...3] are accepted");
}

void DinoPlayer::Update(double timeSinceStart, float deltaTime)
{
    // Gestion des entrées.

    DinoGamepad gamepad{};
    bool bSuccess = XDino_GetGamepad(idxGamepad, gamepad);
    if (bSuccess) {
        bDinoRunning = gamepad.btn_right;
        dinoMove.x = gamepad.stick_left_x;
        dinoMove.y = gamepad.stick_left_y;
        if (gamepad.shoulder_left) {
            bDinoDamage = true;
            dinoDamageStart = timeSinceStart;
        }
    }

    // Logique de jeu.

    if (bDinoDamage && timeSinceStart
        - dinoDamageStart > 3
    )
        bDinoDamage = false;

    float speed = 200;
    if (bDinoRunning)
        speed *= 2;
    if (bDinoDamage)
        speed = 0;

    dinoPos.x += dinoMove.x * speed * deltaTime;
    dinoPos.y += dinoMove.y * speed * deltaTime;
    if (dinoMove.x < 0)
        bDinoLeft = true;
    if (dinoMove.x > 0)
        bDinoLeft = false;
}

void DinoPlayer::Draw(double timeSinceStart, float deltaTime)
{
    DinoDrawCall dino;
    dino.textureName = "dinosaurs.png";
    dino.vertices.emplace_back(DinoVec2{-12, -12}, 0, 0, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{+12, -12}, 24, 0, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{-12, +12}, 0, 24, DinoColor_WHITE);
    dino.vertices.emplace_back(DinoVec2{+12, +12}, 24, 24, DinoColor_WHITE);
    // Mode miroir si on va à gauche.
    if (bDinoLeft) {
        for (DinoVertex& vertex : dino.vertices)
            vertex.u = 24 - vertex.u;
    }

    // Animation
    if (bDinoDamage) {
        // Animation de dégât
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 3;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(336 + 24 * idxFrame);
    }
    else if (dinoMove.x == 0 && dinoMove.y == 0) {
        // Animation sur place
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 4;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(24 * idxFrame);
    }
    else if (bDinoRunning) {
        // Animation de course
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 16) % 6;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(432 + 24 * idxFrame);

    }
    else {
        // Animation de marche
        int64_t idxFrame = static_cast<int64_t>(timeSinceStart * 8) % 6;
        for (DinoVertex& vertex : dino.vertices)
            vertex.u += static_cast<uint16_t>(96 + 24 * idxFrame);
    }

    // Couleur de dinosaure
    for (DinoVertex& vertex : dino.vertices)
        vertex.v += 24 * idxPlayer;

    dino.scale = 2;
    dino.translation = dinoPos;
    XDino_Draw(dino);
}