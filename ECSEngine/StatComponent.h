#pragma once
#include "Component.h"

class StatComponent : public Component
{
public:

	bool hovered;
	float timed;

	StatComponent()
	{
		hovered = false;
		timed = 0;
	}
};