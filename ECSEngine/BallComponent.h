#pragma once
#include "Component.h"
#include "Vec2.h"

class BallComponent : public Component
{
public:

	Vec2 speed;

	BallComponent()
	{
		speed = Vec2(0,0);
	}
};