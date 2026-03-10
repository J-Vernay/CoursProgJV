#include <dino/xdino.h>
#include <dino/DinoController.h>

// Constantes.
constexpr float DINO_SPEED = 300.f; // Nombre de pixels parcourus en une seconde.
constexpr float DINO_RUN_SPEED = 0.5f * DINO_SPEED; // Running Speed
constexpr float DINO_SCALE = 2.f; // Dino Scale

// - All anims lenght
constexpr int ANIM_IDLE_LEN = 4;
constexpr int ANIM_WALK_LEN = 6;
constexpr int ANIM_HURT_LEN = 3;
constexpr int ANIM_RUN_LEN = 6;

constexpr int DINO_ANIM_FRAMES_PER_SECOND = 8;

// - All Corners of Dino Sprite
constexpr DinoVec2 UPPER_LEFT = {0, 0};
constexpr DinoVec2 UPPER_RIGHT = {24, 0};
constexpr DinoVec2 LOWER_LEFT = {0, 24};
constexpr DinoVec2 LOWER_RIGHT = {24, 24};


uint64_t DinoControllerFields::DrawDino(DinoGamepad gamepad, float deltaTime, uint64_t texID_dino)
{
    // Choosing Animation based on current player behaviour
    int currAnimLen;
    int animationSpeed;
    int firstFrameOfAnim;

    // Temp condition to test damage
    if (this->g_dinoCanTakeDamage && gamepad.btn_left || this->dinoDamageAnimTimer > 0.f) {
        currAnimLen = ANIM_HURT_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + 4;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;

        if (this->g_dinoCanTakeDamage) {
            this->dinoDamageAnimTimer = ANIM_HURT_LEN;
            this->g_dinoCanTakeDamage = false;
        }
        else {
            this->dinoDamageAnimTimer -= deltaTime;
            if (this->dinoDamageAnimTimer <= 0.f) {
                this->g_dinoCanTakeDamage = true;
            }
        }
    }
    else if (gamepad.stick_left_x == 0.0f && gamepad.stick_left_y == 0.0f) {
        currAnimLen = ANIM_IDLE_LEN;
        firstFrameOfAnim = 0;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    }
    else if (gamepad.btn_right == 0.0f) {
        currAnimLen = ANIM_WALK_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND;
    }
    else {
        currAnimLen = ANIM_RUN_LEN;
        firstFrameOfAnim = ANIM_IDLE_LEN + ANIM_WALK_LEN + ANIM_HURT_LEN + 4 + 1;
        animationSpeed = DINO_ANIM_FRAMES_PER_SECOND * 2;
    }

    if (this->dinoAnimElapsed > 1.f / animationSpeed) {
        this->currFrame = (this->currFrame + 1) % currAnimLen;
        this->dinoAnimElapsed = 0.0f;
    }
    else {
        this->dinoAnimElapsed += deltaTime;
    }

    // Displaying Dino

    // Only change facing orientation if player is moving
    if (gamepad.stick_left_x != 0.0f) {
        this->dinoGoingLeft = gamepad.stick_left_x <= 0.0f; // Changes facing orientation based on input
    }

    std::vector<DinoVertex> vs;
    vs.resize(6);

    vs[0].pos = this->dinoGoingLeft ? UPPER_RIGHT : UPPER_LEFT;
    vs[0].u = 0 + (firstFrameOfAnim + this->currFrame) * 24;
    vs[0].v = 0 + this->dinoColor * 24;

    vs[1].pos = this->dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
    vs[1].u = 24 + (firstFrameOfAnim + this->currFrame) * 24;
    vs[1].v = 0 + this->dinoColor * 24;

    vs[2].pos = this->dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
    vs[2].u = 0 + (firstFrameOfAnim + this->currFrame) * 24;
    vs[2].v = 24 + this->dinoColor * 24;

    vs[3].pos = this->dinoGoingLeft ? UPPER_LEFT : UPPER_RIGHT;
    vs[3].u = 24 + (firstFrameOfAnim + this->currFrame) * 24;
    vs[3].v = 0 + this->dinoColor * 24;

    vs[4].pos = this->dinoGoingLeft ? LOWER_LEFT : LOWER_RIGHT;
    vs[4].u = 24 + (firstFrameOfAnim + this->currFrame) * 24;
    vs[4].v = 24 + this->dinoColor * 24;

    vs[5].pos = this->dinoGoingLeft ? LOWER_RIGHT : LOWER_LEFT;
    vs[5].u = 0 + (firstFrameOfAnim + this->currFrame) * 24;
    vs[5].v = 24 + this->dinoColor * 24;

    this->vbufID_dino = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Dino");
    XDino_Draw(this->vbufID_dino, texID_dino, this->dinoPos, DINO_SCALE);

    return this->vbufID_dino;
}

void DinoControllerFields::DinoMovement(DinoGamepad gamepad, float deltaTime)
{
    // Prevent Diagonal speed boost
    if (gamepad.stick_left_x != 0.0f && gamepad.stick_left_y != 0.0f)
        this->dinoCurrentSpeed = 0.75f * DINO_SPEED;
    else
        this->dinoCurrentSpeed = DINO_SPEED;

    DinoVec2 dinoPosDelta = {0, 0};
    dinoPosDelta.x += gamepad.stick_left_x * (this->dinoCurrentSpeed + gamepad.btn_right * DINO_RUN_SPEED) *
        deltaTime;
    dinoPosDelta.y += gamepad.stick_left_y * (this->dinoCurrentSpeed + gamepad.btn_right * DINO_RUN_SPEED) *
        deltaTime;

    this->dinoPos.x += dinoPosDelta.x;
    this->dinoPos.y += dinoPosDelta.y;
}