#include "Physics.h"

std::vector<Collider*> Physics::colliders = {};

void Physics::Update(float deltaTime)
{
    for (int firstColliderIndex = 0; firstColliderIndex < colliders.size(); ++firstColliderIndex)
    {
        for (int secondColliderIndex = firstColliderIndex + 1; secondColliderIndex < colliders.size(); ++secondColliderIndex)
        {
            Collider& firstCollider  = *colliders[firstColliderIndex];
            Collider& secondCollider = *colliders[secondColliderIndex];
    
        
            
            DinoVec2 firstPosition  = firstCollider.GetPosition();
            DinoVec2 secondPosition = secondCollider.GetPosition();
    
            float dx = secondPosition.x - firstPosition.x;
            float dy = secondPosition.y - firstPosition.y;
    
            float distance = sqrt(dx * dx + dy * dy);
            float minimumDistance = firstCollider.collider_radius + secondCollider.collider_radius;
            
            if (distance < minimumDistance && distance > 1.0f)
            {
                float penetrationDepth = minimumDistance - distance;
            
                float normalX = dx / distance;
                float normalY = dy / distance;
                
                float separationX = normalX * penetrationDepth * 0.5f;
                float separationY = normalY * penetrationDepth * 0.5f;
            
                firstPosition.x  -= separationX;
                firstPosition.y  -= separationY;
            
                secondPosition.x += separationX;
                secondPosition.y += separationY;
            
                firstCollider.SetPosition(firstPosition);
                secondCollider.SetPosition(secondPosition);
            }
        }
    }
}