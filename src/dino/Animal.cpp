#include "Animal.h"

#include "Physics.h"
#include "Rendering.h"

#include <cmath>
#include <numbers>
#include <algorithm>
#include <dino/dino_terrain.h>

uint64_t Animal::texID_Texture = 0;

void Animal::Init()
{
    Physics::colliders.push_back(this);
    Rendering::spriteRenderer.push_back(this);
    Pos = DefineDestination();
    Destination = DefineDestination();
    collider_radius = 8;
    
}


 std::vector<DinoVertex> Animal::FadeIn(float deltaTime, std::vector<DinoVertex> vs)
{
    uint8_t color = (1 - timeSinceSpawn) * 255;
    timeSinceSpawn -= deltaTime;
    for (DinoVertex& v : vs)
    {
        v.color = {255,255,255,color};
    }

    if (timeSinceSpawn <= 0)isFadeInDone = true;
    return vs;
}

std::vector<DinoVertex> Animal::FadeOut(float deltaTime, std::vector<DinoVertex> vs)
{
    fadeOutTimer -= deltaTime;

    float t = std::max(0.f, fadeOutTimer);
    uint8_t color = static_cast<uint8_t>(t * 255);

    for (DinoVertex& v : vs)
        v.color = {255,255,255,color};

    if (fadeOutTimer <= 0)
        fadeOutDone = true;

    return vs;
}



void Animal::Update(double deltaTime)
{

    if (vbufID_Texture != 0) {
        XDino_DestroyVertexBuffer(vbufID_Texture);
        vbufID_Texture = 0;
    }
    
    std::vector<DinoVertex> vs = {};
    Dino_GenVertices_Animal(vs ,EAnimalKind::Sheep1,ChooseAnim(),deltaTime);

    
    if (!isFadeInDone)vs = FadeIn(deltaTime,vs);
    if (isFadeInDone && !isAlive && !fadeOutDone) {
        
        if (vs[1].color.a <= 0) {
            fadeOutDone = true;
        }
        else {
            vs = FadeOut(deltaTime,vs);
        }
    }
    

    vbufID_Texture = XDino_CreateVertexBuffer(vs.data(), vs.size(), "Animal");
    //Draw();



    if (Distance(Pos, Destination) < distanceThreshold)
        Destination = DefineDestination();
    Pos = MoveTowards(Pos, Destination, Speed, deltaTime);



}



float Animal::Distance(DinoVec2 a, DinoVec2 b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;

    return sqrt(dx * dx + dy * dy);
}

DinoVec2 Animal::DefineDestination()
{
    float x = XDino_RandomFloat(DinoTerrain::FenceStart.x, DinoTerrain::FenceStart.x + DinoTerrain::Fence.x);
    float y = XDino_RandomFloat(DinoTerrain::FenceStart.y, DinoTerrain::FenceStart.y + DinoTerrain::Fence.y);

    return {x, y};
}

DinoVec2 Animal::MoveTowards(DinoVec2 pos, DinoVec2 destination, float speed, float deltatime)
{
    DinoVec2 direction;

    direction.x = destination.x - pos.x;
    direction.y = destination.y - pos.y;

    float len = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (len > 0)
    {
        direction.x /= len;
        direction.y /= len;
    }

    pos.x += direction.x * speed * deltatime;
    pos.y += direction.y * speed * deltatime;

    return pos;
}


EAnimalAnim Animal::ChooseAnim()
{
    float angle = atan2(Pos.y - Destination.y, Pos.x - Destination.x) * 180 / std::numbers::pi_v<float>;

    if (angle >= -45 && angle < 45)
        return EAnimalAnim::Left;

    if (angle >= 45 && angle < 135)
        return EAnimalAnim::Up;

    if (angle >= -135 && angle < -45)
        return EAnimalAnim::Down;

    return EAnimalAnim::Right;
}

void Animal::Draw()
{
    if (vbufID_Texture == 0)
        return;

    DinoVec2 spriteDest = {Pos.x - 16, Pos.y - 32};
    XDino_Draw(vbufID_Texture, texID_Texture, spriteDest, 1);
}




void Animal::Shut()
{
    if (vbufID_Texture != 0)
    {
        XDino_DestroyVertexBuffer(vbufID_Texture);
        vbufID_Texture = 0;
    }

    auto& colliderList = Physics::colliders;
    colliderList.erase(std::remove(colliderList.begin(), colliderList.end(), this), colliderList.end());

    auto& renderList = Rendering::spriteRenderer;
    renderList.erase(std::remove(renderList.begin(), renderList.end(), this), renderList.end());
}



void Animal::InitStatic(uint64_t textID)
{
    texID_Texture = textID;
}

void Animal::ShutStatic()
{
    XDino_DestroyGpuTexture(texID_Texture);
}
