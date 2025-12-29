#pragma once
#include "Component.h"

class ProjectileComponent : public Component
{
public:

	int damage;
	std::shared_ptr<Entity> target;
	int speed;

	ProjectileComponent()
	{
		damage = 0;
		target = nullptr;
		speed = 5;
	}
	ProjectileComponent(int d, std::shared_ptr<Entity> t, int s) : damage(d), target(t), speed(s)
	{
	}
};