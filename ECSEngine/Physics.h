#pragma once
#include "GameManager.h"
#include "ComponentHeaders.h"

class Physics
{
public:

	Vec2 getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2 getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2 midPos(std::shared_ptr<Entity> a, Vec2& pos);
};

