#pragma once
#include "Component.h"
#include <string>

class StateComponent : public Component
{
public:

	std::string state = "onAir";

	StateComponent()
	{
	}

	StateComponent(std::string& s)
	{
		state = s;
	}
};