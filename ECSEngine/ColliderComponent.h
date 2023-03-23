#pragma once
#include "Component.h"
#include "Vec2.h"

class ColliderComponent : public Component
{
public:

	Vec2 size;
	Vec2 halfSize;

	ColliderComponent()
	{
	}

	ColliderComponent(const Vec2& s) : size(s), halfSize(s.x / 2.0f, s.y / 2.0f)
	{
	}
};