#pragma once

#include <format>
#include <unordered_map>
#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>
#include <dino/Entities/dino_player.h>

class dino_ScoreManager {
public :
    void ResetScores(std::unordered_map<DinoGamepadIdx, dino_player>& gamepadDino_map);
    void AddPlayer(int id);
    void RemovePlayer(int id);
    int AddScore(int playerId, EAnimalKind animalType);
    void DrawScores();

private :
    std::unordered_map<EAnimalKind, int> catchedAnimalAmount_map;
    std::unordered_map<int, int> playerScore_map;
};