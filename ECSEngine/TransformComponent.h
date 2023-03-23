#pragma once
#include "Component.h"

class TransformComponent : public Component
{
public:

	Vec2 position;
	Vec2 prevPosition;
	Vec2 velocity;
	float angle;

	TransformComponent()
	{
		position.Zero();
		prevPosition.Zero();
		velocity.Zero();
		angle = 0;
	}

	TransformComponent(const Vec2& p, const Vec2& v, float a) : position(p), prevPosition(p), velocity(v), angle(a)
	{
	}
};