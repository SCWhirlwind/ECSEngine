#include "Physics.h"
#include "Scene_Platformer.h"

Vec2 Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (a->hasComponent<ColliderComponent>() && b->hasComponent<ColliderComponent>() &&
        a->hasComponent<TransformComponent>() && b->hasComponent<TransformComponent>())
    {
        auto& aBBox = a->getComponent<ColliderComponent>();
        auto& bBBox = b->getComponent<ColliderComponent>();
        auto& aTransform = a->getComponent<TransformComponent>();
        auto& bTransform = b->getComponent<TransformComponent>();

        Vec2 aMidPos = midPos(a, aTransform.position);
        Vec2 bMidPos = midPos(b, bTransform.position);

        float deltax = abs(aMidPos.x - bMidPos.x);
        float ox = (aBBox.halfSize.x + bBBox.halfSize.x) - deltax;
        float deltay = abs(aMidPos.y - bMidPos.y);
        float oy = (aBBox.halfSize.y + bBBox.halfSize.y) - deltay;

        return Vec2(ox, oy);
    }
    return Vec2(0, 0);
}

Vec2 Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (a->hasComponent<ColliderComponent>() && b->hasComponent<ColliderComponent>() &&
        a->hasComponent<TransformComponent>() && b->hasComponent<TransformComponent>())
    {
        auto& aBBox = a->getComponent<ColliderComponent>();
        auto& bBBox = b->getComponent<ColliderComponent>();
        auto& aTransform = a->getComponent<TransformComponent>();
        auto& bTransform = b->getComponent<TransformComponent>();

        Vec2 aMidPos = midPos(a, aTransform.prevPosition);
        Vec2 bMidPos = midPos(b, bTransform.prevPosition);

        float deltax = abs(aMidPos.x - bMidPos.x);
        float ox = (aBBox.halfSize.x + bBBox.halfSize.x) - deltax;
        float deltay = abs(aMidPos.y - bMidPos.y);
        float oy = (aBBox.halfSize.y + bBBox.halfSize.y) - deltay;

        return Vec2(ox, oy);
    }
    return Vec2(0, 0);
}

Vec2 Physics::midPos(std::shared_ptr<Entity> a, Vec2& pos)
{
    if (a->hasComponent<SpriteComponent>())
    {
        Vec2 size = Vec2(0,0);
        size.x = a->getComponent<SpriteComponent>().width * a->getComponent<SpriteComponent>().scale;
        size.y = a->getComponent<SpriteComponent>().height * a->getComponent<SpriteComponent>().scale;

        return Vec2(pos.x + size.x/ 2.0f, pos.y + size.y / 2.0f);
    }

    return Vec2(0, 0);
}
