#pragma once

#include <format>
#include <unordered_map>
#include <dino/dino_geometry.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_player.h>
#include <dino/dino_terrain.h>

class DinoScoreManager {
public :
    void ResetScores();
    void AddPlayer(int id);
    void RemovePlayer(int id);
    int AddScore(int playerId, EAnimalKind animalType);
    void EndAnimalBonus();
    void DrawScores(DinoTerrain& terrain);

private :
    std::unordered_map<int, int> catchedAnimalAmount_map;
    std::unordered_map<int, int> playerScore_map;
};