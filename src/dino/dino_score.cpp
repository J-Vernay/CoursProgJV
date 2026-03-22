#include <dino/dino_score.h>

void DinoScoreManager::ResetScores()
{
    catchedAnimalAmount_map.clear();
    for (auto& [id, score] : playerScore_map) {
        score = 0;
    }
}

void DinoScoreManager::AddPlayer(int id)
{
    playerScore_map[id] = 0;
}

void DinoScoreManager::RemovePlayer(int id)
{
    playerScore_map.erase(id);
}

int DinoScoreManager::AddScore(int playerId, EAnimalKind animalType)
{
    int animalID = 0;
    switch (animalType) {
    case EAnimalKind::Cow1:
    case EAnimalKind::Cow2: animalID = 0;
        break;
    case EAnimalKind::Ostrich1:
    case EAnimalKind::Ostrich2: animalID = 1;
        break;
    case EAnimalKind::Pig1:
    case EAnimalKind::Pig2: animalID = 2;
        break;
    case EAnimalKind::Sheep1:
    case EAnimalKind::Sheep2: animalID = 3;
        break;
    default: animalID = 0;
        break;

    }
    catchedAnimalAmount_map[animalID] += 1;
    if (playerScore_map.contains(playerId)) {
        int scoreToAdd = catchedAnimalAmount_map[animalID] * 10;
        playerScore_map[playerId] += scoreToAdd;
        return scoreToAdd;
    }
    return 0;
}

void DinoScoreManager::EndAnimalBonus()
{
    catchedAnimalAmount_map.clear();
}

void DinoScoreManager::DrawScores(DinoTerrain& terrain)
{
    for (std::pair<const int, int>& playerScore : playerScore_map) {
        DinoColor textColor =
            playerScore.first == 0
                ? DinoColor_BLUE
                : playerScore.first == 1
                ? DinoColor_RED
                : playerScore.first == 2
                ? DinoColor_YELLOW
                : DinoColor_GREEN;

        std::vector<DinoVertex> vs;
        std::string text = " Player " + std::to_string(playerScore.first + 1) + " \n " + std::to_string(
                               playerScore.second);

        Dino_GenVertices_Text(
            vs,
            text,
            textColor,
            DinoColor_BLACK
        );

        DinoVertexBuffer new_vertex_buffer = {vs.data(), vs.size(), "PlayerScore"};

        DinoVec2 topLeft = terrain.GetTopLeft();
        DinoVec2 bottomRight = terrain.GetBottomRight();
        float ty = bottomRight.y / 4;
        float x = 0;
        float y = topLeft.y + playerScore.first * ty;

        XDino_Draw(new_vertex_buffer.GetVbufID(),
                   XDino_TEXID_FONT,
                   {x, y},
                   1.2f);
    }
}