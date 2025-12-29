#pragma once
#include "Component.h"

class TowerComponent : public Component
{
public:

	int damage;
	int bonus;
	int range;
	float attackSpeed;
	float attackTimer = 0.0f;

	std::vector<int> enemiesIDInRange;

	TowerComponent()
	{
		damage = 0;
		bonus = 0;
		range = 0;
		attackSpeed = 0.0f;
		attackTimer = 0.0f;
	}
	TowerComponent(int d, int b, int r, float a) : damage(d), bonus(b), range(r), attackSpeed(a)
	{
	}
};