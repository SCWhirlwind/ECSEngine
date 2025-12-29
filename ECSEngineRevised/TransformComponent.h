#pragma once
#include "Component.h"

class TransformComponent : public Component
{
public:

	Vec2 position;
	Vec2 prevPosition;

	TransformComponent()
	{
		position.Zero();
		prevPosition.Zero();
	}

	TransformComponent(const Vec2& p, const Vec2& v, float a) : position(p), prevPosition(p)
	{
	}
};