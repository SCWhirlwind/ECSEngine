#pragma once
#include "Component.h"

class ButtonComponent : public Component
{
public:

	bool isActive = false;
	bool isClicked = false;

	ButtonComponent()
	{
	}

};