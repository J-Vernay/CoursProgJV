#pragma once
#include <dino/xdino.h>
#include <dino/dino_player.h>

void ExchangeValues(uint16_t* value1, uint16_t* value2);
void InstantiateDinoVerticles(DinoDrawCall* drawCallDino);
void SwitchDinoRotation(DinoDrawCall* drawCallDino);
void DetectMovement(DinosaurPlayer& dinosaur, bool movementRegistered, float xMovement, float yMovement);