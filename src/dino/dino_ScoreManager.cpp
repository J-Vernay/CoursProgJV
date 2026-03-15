#include <dino/dino_ScoreManager.h>

void dino_ScoreManager::ResetScores(std::unordered_map<DinoGamepadIdx, dino_player>& gamepadDino_map)
{
    catchedAnimalAmount_map.clear();
    for (std::pair<const DinoGamepadIdx, dino_player>& gamepadDino : gamepadDino_map) {
        playerScore_map[gamepadDino.second.dinoID] = 0;
    }
}

void dino_ScoreManager::AddPlayer(int id)
{
    playerScore_map[id] = 0;
}

void dino_ScoreManager::RemovePlayer(int id)
{
    playerScore_map.erase(id);
}

int dino_ScoreManager::AddScore(int playerId, EAnimalKind animalType)
{
    catchedAnimalAmount_map[animalType] += 1;
    if (playerScore_map.contains(playerId)) {
        int scoreToAdd = catchedAnimalAmount_map[animalType] * 10;
        playerScore_map[playerId] += scoreToAdd;
        return scoreToAdd;
    }
    return 0;
}

void dino_ScoreManager::DrawScores()
{
    DinoVec2 offset = {0, 180.0f - ((int)playerScore_map.size() / 2) * 23};
    int i = 0;
    for (std::pair<const int, int>& playerScore : playerScore_map) {
        std::string text = std::format("P{0}:{1:04}", playerScore.first, playerScore.second);
        DinoColor textColor =
            playerScore.first == 0
                ? DinoColor_BLUE
                : playerScore.first == 1
                ? DinoColor_RED
                : playerScore.first == 2
                ? DinoColor_YELLOW
                : DinoColor_GREEN;

        std::vector<DinoVertex> vs;
        DinoVec2 textSize = Dino_GenVertices_Text(vs, text, textColor, DinoColor_BLACK);
        DinoVec2 position = DinoVec2{0, -textSize.y + 23 * i} + offset;
        DinoVertexBuffer vbufID(vs.data(), vs.size(), "playerScore");
        XDino_Draw(vbufID.Get(), XDino_TEXID_FONT, position, 2);
        i++;
    }
}