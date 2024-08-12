#pragma once
#include "Component.h"

class GolComponent : public Component
{
public:

	bool previousState = false;
	bool currentState = false;

	int framecounter = 0;

	GolComponent()
	{
	}
};