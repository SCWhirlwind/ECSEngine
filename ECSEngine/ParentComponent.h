#pragma once
#include "Component.h"

class ParentComponent : public Component
{
public:

	std::shared_ptr<Entity> parent;

	ParentComponent()
	{
		parent = nullptr;
	}

	ParentComponent(std::shared_ptr<Entity> p)
	{
		parent = p;
	}
};