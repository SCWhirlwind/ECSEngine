#pragma once
#include "Component.h"
#include "Vec2.h"

class PointComponent : public Component
{
public:

	Vec2 point;

	PointComponent()
	{
		point = Vec2(0, 0);
	}

	PointComponent(Vec2 p) : point(p)
	{
	}
};