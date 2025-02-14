#include <dino/dino_movement.h>

void ExchangeValues(uint16_t* value1, uint16_t* value2)
{
    uint16_t e = *value1;
    *value1 = *value2;
    *value2 = e;
}
void InstantiateDinoVerticles(DinoDrawCall* drawCallDino)
{
    drawCallDino->vertices.resize(6);
    drawCallDino->vertices[0].pos = {0, 0};
    drawCallDino->vertices[0].u = 4;drawCallDino->vertices[0].v = 4;
        
    drawCallDino->vertices[1].pos = {15, 0};
    drawCallDino->vertices[1].u = 19;drawCallDino->vertices[1].v = 4;
        
    drawCallDino->vertices[2].pos = {0, 17};
    drawCallDino->vertices[2].u = 4;drawCallDino->vertices[2].v = 21;

    drawCallDino->vertices[3].pos = {15, 17};
    drawCallDino->vertices[3].u = 19;drawCallDino->vertices[3].v = 21;
        
    drawCallDino->vertices[4].pos = {15, 0};
    drawCallDino->vertices[4].u = 19;drawCallDino->vertices[4].v = 4;
        
    drawCallDino->vertices[5].pos = {0, 17};
    drawCallDino->vertices[5].u = 4;drawCallDino->vertices[5].v = 21;
}
void SwitchDinoRotation(DinoDrawCall* drawCallDino)
{
    ExchangeValues(&drawCallDino->vertices[0].u, &drawCallDino->vertices[3].u);
    ExchangeValues(&drawCallDino->vertices[1].u, &drawCallDino->vertices[5].u);
    ExchangeValues(&drawCallDino->vertices[2].u, &drawCallDino->vertices[4].u);
}

bool TakeHit(DinoDrawCall* drawCallDino, float* timeToWait)
{
    //Need to Do
    return true;
}

void DetectMovement(DinosaurPlayer& dinosaur, bool movementRegistered, float xMovement, float yMovement){
    if(movementRegistered) {
        dinosaur.counter+= 0.1f;
        
        float cSpeed = dinosaur.speed;
        
        if (dinosaur.counter > dinosaur.timeUntilDoubleSpeed) {
            cSpeed*=2;
        }
        dinosaur.dinoPos.x += xMovement * cSpeed;
        dinosaur.dinoPos.y += yMovement * cSpeed;
    }
    else {
        dinosaur.counter = 0;
    }
}