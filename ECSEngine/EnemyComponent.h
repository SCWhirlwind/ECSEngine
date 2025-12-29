#pragma once
#include "Component.h"

class EnemyComponent : public Component
{
public:

	int hp;
	int speed;

	int waypointIndex;

	EnemyComponent()
	{
		hp = 0;
		speed = 0;
		waypointIndex = 0;
	}
	EnemyComponent(int h, int s, int w) : hp(h), speed(s), waypointIndex(w)
	{
	}
};