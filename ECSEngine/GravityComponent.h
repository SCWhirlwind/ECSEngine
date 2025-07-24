#pragma once
#include "Component.h"
#include "Vec2.h"

class GravityComponent : public Component
{
public:

	int weight;

	GravityComponent()
	{
		weight = 1;
	}

	GravityComponent(int w)
	{
		weight = w;
	}
};