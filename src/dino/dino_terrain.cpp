#include <dino/xdino.h>
#include <dino/dino_draw_utils.h>
#include <dino/dino_terrain.h>

constexpr int TERRAIN_WIDTH_TOTAL = TERRAIN_WIDTH + 2;
constexpr int TERRAIN_HEIGHT_TOTAL = TERRAIN_HEIGHT + 2;
constexpr float ANIMATION_FRAMERATE = 4;

const int terrainAnimationFrames[] = {
    0, 1, 2, 1, 0
};

constexpr DinoColor oceanColors[] = {
    {0, 146, 221, 255},
    {0, 146, 221, 255},
    {0, 146, 221, 255},
    {153, 207, 242, 255}
};

void DinoTerrain::init()
{
    season = static_cast<Season>(XDino_RandomInt32(0, WINTER));

    for (int y = 0; y < TERRAIN_HEIGHT; y++) {
        for (int x = 0; x < TERRAIN_WIDTH; x++) {
            int16_t tile = XDino_RandomInt32(0, 20);
            if (tile > 4) {
                tile = 0;
            }
            decorations[y * TERRAIN_WIDTH + x] = tile;
        }
    }
}

void DinoTerrain::update(float deltaTime)
{
    timer += deltaTime;
    if (timer < 1 / ANIMATION_FRAMERATE)
        return;
    timer = 0;
    frame = (frame + 1) % (sizeof(terrainAnimationFrames) / sizeof(int));
}


void DinoTerrain::draw_ocean()
{
    XDino_SetClearColor(oceanColors[season]);
}


void DinoTerrain::draw_terrain()
{
    DinoVec2 renderSize = XDino_GetRenderSize();
    DinoVec2 offset = {(renderSize.x - TERRAIN_WIDTH_TOTAL * 16) / 2.0f,
                       (renderSize.y - TERRAIN_HEIGHT_TOTAL * 16) / 2.0f};
    uint16_t uSeason = season * 5;

    for (int x = 0; x < TERRAIN_WIDTH_TOTAL; x++) {
        int16_t u = 1;

        if (x == 0) {
            u = 0;
        }
        else if (x == TERRAIN_WIDTH_TOTAL - 1) {
            u = 2;
        }

        DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(u + uSeason, 1 + terrainAnimationFrames[frame] * 3, 16, 16);
        drawCall.textureName = "terrain.png";
        drawCall.translation = {offset.x + x * 16, offset.y};
        XDino_Draw(drawCall);

        drawCall = Dino_CreateDrawCall_Sprite(u + uSeason, 3 + terrainAnimationFrames[frame] * 3, 16, 16);
        drawCall.textureName = "terrain.png";
        drawCall.translation = {offset.x + x * 16, offset.y + 13 * 16};
        XDino_Draw(drawCall);
    }

    for (int y = 0; y < TERRAIN_HEIGHT; y++) {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(uSeason, 2 + terrainAnimationFrames[frame] * 3, 16, 16);
        drawCall.textureName = "terrain.png";
        drawCall.translation = {offset.x, offset.y + (y + 1) * 16};
        XDino_Draw(drawCall);

        drawCall = Dino_CreateDrawCall_Sprite(2 + uSeason, 2 + terrainAnimationFrames[frame] * 3, 16, 16);
        drawCall.textureName = "terrain.png";
        drawCall.translation = {offset.x + (TERRAIN_WIDTH_TOTAL - 1) * 16, offset.y + (y + 1) * 16};
        XDino_Draw(drawCall);
    }

    {
        DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(1 + uSeason, 0, 16, 16);

        drawCall.vertices[1].pos.y = TERRAIN_HEIGHT * 16;
        drawCall.vertices[2].pos = {TERRAIN_WIDTH * 16, TERRAIN_HEIGHT * 16};
        drawCall.vertices[3].pos = {TERRAIN_WIDTH * 16, TERRAIN_HEIGHT * 16};
        drawCall.vertices[4].pos.x = TERRAIN_WIDTH * 16;

        drawCall.textureName = "terrain.png";
        drawCall.translation = {offset.x + 16, offset.y + 16};
        XDino_Draw(drawCall);
    }

    for (int y = 0; y < TERRAIN_HEIGHT; y++) {
        for (int x = 0; x < TERRAIN_WIDTH; x++) {
            if (decorations[y * TERRAIN_WIDTH + x] == 0)
                continue;

            DinoDrawCall drawCall = Dino_CreateDrawCall_Sprite(decorations[y * TERRAIN_WIDTH + x] + uSeason, 0, 16, 16);
            drawCall.textureName = "terrain.png";
            drawCall.translation = {offset.x + (x + 1) * 16, offset.y + (y + 1) * 16};
            XDino_Draw(drawCall);
        }
    }
}

DinoVec2 DinoTerrain::get_terrain_min_position()
{
    DinoVec2 renderSize = XDino_GetRenderSize();
    return {(renderSize.x - TERRAIN_WIDTH * 16) / 2.0f, (renderSize.y - TERRAIN_HEIGHT * 16) / 2.0f};
}

DinoVec2 DinoTerrain::get_terrain_max_position()
{
    DinoVec2 minPosition = get_terrain_min_position();
    return {minPosition.x + TERRAIN_WIDTH * 16, minPosition.y + TERRAIN_HEIGHT * 16};
}